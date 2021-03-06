// Always include the header of the unit first
#include "game.h"
#include "tile_id.h"
#include "sfml_resources.h" //NOTE Until we have hitboxes
#include "sfml_camera.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <QFile>
#include <algorithm>
#include <functional>
#include <chrono>
#include <random>

game::game(
  const std::vector<tile>& tiles,
  const std::vector<agent>& agents
) : m_allow_spawning{false},
    m_allow_damage{true},
    m_tiles{tiles},
    m_agents{agents},
    m_n_tick{0},
    m_score{0},
    m_essence{0},
    m_sound_type{sound_type::none}
{
  assert(m_sound_type == sound_type::none);
}

void game::add_agents(const std::vector<agent>& as)
{
  std::copy(
    std::begin(as),
    std::end(as),
    std::back_inserter(m_agents)
  );
}

void game::add_tiles(const std::vector<tile>& ts)
{
  std::copy(
    std::begin(ts),
    std::end(ts),
    std::back_inserter(m_tiles)
  );
}

std::vector<tile_type> collect_tile_types(const game& g) noexcept
{
  std::vector<tile_type> types;
  for (const tile& t: g.get_tiles())
  {
    types.push_back(t.get_type());
  }
  return types;
}

std::default_random_engine device(time(NULL));
int random_int(int min, int max){
    std::minstd_rand generator(device());
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}

double random_double(double min, double max){
    std::minstd_rand generator(device());
    std::uniform_real_distribution<double> distribution(min, max);

    return distribution(generator);
}

int count_n_tiles(const game& g) noexcept
{
  return g.get_tiles().size();
}

int count_n_agents(const game& g) noexcept
{
  return g.get_agents().size();
}

void game::process_events(sound_type& st)
{
  set_sound_type(sound_type::none);
  assert(m_sound_type == sound_type::none);

  //std::clog << "Reproduction\n";
  {
    std::vector<agent> all_kids;

    //m_agents should not change in size during this loop!
    const int n_agents_before = static_cast<int>(m_agents.size());

    for (auto& a: m_agents)
    {
      const std::vector<agent> kids = a.process_events(*this);
      std::copy(
        std::begin(kids),
        std::end(kids),
        std::back_inserter(all_kids)
      );

      //m_agents should not change in size during this loop!
      const int n_agents_in_loop = static_cast<int>(m_agents.size());
      assert(n_agents_before == n_agents_in_loop);
    }
    add_agents(all_kids);
  }

  //std::clog << "Count plants\n";
  int agent_count = 0;
  for (const auto& a: m_agents) { //BUG: m_agents changes size
    if (!is_plant(a.get_type())) {
      agent_count++;
    }
  }

  //std::clog << "If an agent has less than zero health, turn it into a corpse\n";
  if(m_allow_damage)
  {
    kill_agents();
  }


  //std::clog << "Remove all corpses in m_agents\n";
  {
    for (int i = 0; i < static_cast<int>(m_agents.size()); ++i)
    {
      assert(i >= 0);
      assert(i < static_cast<int>(m_agents.size()));
      agent& a = m_agents[i];
      //Agent is a corpse that needs to be removed at this tick
      if(a.get_type() == agent_type::corpse
        && a.get_corpse_ticks() + 300 < this->get_n_ticks()
        && a.get_corpse_ticks() >= 0
      )
      {
        m_agents[i] = m_agents.back();
        m_agents.pop_back();
        --i; // Retry this index with the new agent
      }
    }
  }

  //std::clog << "Merge tiles\n";
  merge_tiles(st);

  //std::clog << "Calculate the score\n";
  double ppt = agent_count;
  if (m_tiles.size() != 0) {
    assert(m_tiles.size() > 0);
    ppt = ppt / m_tiles.size();
  }
  if(m_allow_score) {
    m_score = ppt * 112 - 112;
  }

  //std::clog << "Calculate the score\n";
  if (m_n_tick % 100 == 0) {
     m_essence += 112 - std::abs(m_score);
  }

  //std::clog << "Process the events happening on the tiles\n";
  for (auto& tile : m_tiles)
  {
    if(tile.get_dx() != 0 || tile.get_dy() != 0) {
      tile.move(m_agents);
    }
    tile.process_events(*this);
  }

  // DO NOT DO FOR AGENT IN GET_AGENTS HERE

  //std::clog << "Adding a tick\n";
  ++m_n_tick;
}

void game::tile_merge(tile& focal_tile, const tile& other_tile, const int other_pos) {
  // Merge attempt with this function
  const std::vector<tile_type> merged_types = get_merge_type(
    focal_tile.get_type(),
    other_tile.get_type()
  );
  if (merged_types.empty()) return;
  //focal tile becomes merged type
  const tile_type merged_type = merged_types[0];
  focal_tile.set_type(merged_type);
  //other tile is swapped to the back, then deleted
  m_tiles[other_pos] = m_tiles.back();
  m_tiles.pop_back();
  //change the selected tile
  m_selected.clear();
  assert(m_selected.empty());
}

void game::check_selection(double mouse_X, double mouse_y){

  // Check is there is a tile on mouse-click coordinates
  for (unsigned i = 0; i < m_tiles.size(); i++)
  {
    if (contains(m_tiles.at(i),
          mouse_X, mouse_y))
    {
      for (unsigned j = 0; j < m_tiles.size(); j++)
      {
        // If some tiles are moving, don't select
        if (m_tiles.at(j).get_dx() != 0 || m_tiles.at(j).get_dy() != 0){
          return;
        }
      }
      // Set the tile whithin mouse-click coordinates as the selected tile
      tile s_tile = m_tiles.at(i);
      m_selected.clear();
      m_selected.push_back(s_tile.get_id());
      return;
    }
  }

  // If there's no tile at the mouse-selection coordinates check if some tiles are moving

  for (unsigned i = 0; i < m_tiles.size(); i++)
  {
    // If there are moving tiles, return
    if (m_tiles.at(i).get_dx() != 0 || m_tiles.at(i).get_dy() != 0){
      return;
    }
  }

  // Else clear selection
  m_selected.clear();
}

void game::merge_tiles(sound_type &st) { //!OCLINT must simplify
  // I use indices here, so it is more beginner-friendly
  // one day, we'll use iterators
  const int n = count_n_tiles(*this);
  for (int i = 0; i < n - 1; ++i)
  // n - 1 because two elements
  {
    assert(i >=0);
    assert(i < static_cast<int>(m_tiles.size()));
    tile& focal_tile = m_tiles[i];
    // j is the next tile in the vector
    for (int j = i + 1; j < n; ++j)
    {
      assert(j >=0);
      assert(j < static_cast<int>(m_tiles.size()));
      tile& other_tile = m_tiles[j];
      if (!have_same_position(focal_tile, other_tile)) { continue; }
      tile_merge(focal_tile, other_tile, j);
      st = sound_type::tile_collision;
      return; //!OCLINT I don't know an alternative;
      i = n; j = n;
    }
  }
}

void game::kill_agents()
{
  for (int i = 0; i < static_cast<int>(m_agents.size()); ++i) {
    assert(i >= 0);
    assert(i < static_cast<int>(m_agents.size()));

    if (m_agents[i].get_health() <= 0.0 && m_agents[i].get_type() != agent_type::corpse) {
      sf::Vector2f center1 = get_agent_center(m_agents[i]);
      agent a(agent_type::corpse, center1.x, center1.y);
      double width = get_agent_width(sfml_resources::get().get_agent_sprite(a.get_type())) * 0.2f;
      double height = get_agent_height(sfml_resources::get().get_agent_sprite(a.get_type())) * 0.2f;
      a.set_x(a.get_x() - width/2);
      a.set_y(a.get_y() - height/2);

      if(!is_plant(m_agents[i].get_type())) {
        m_agents.push_back(a); }

      m_agents[i] = m_agents.back();            
      m_agents.pop_back();
      --i; //Retry this index with the new agent
    }
  }
}

void game::remove_tile(double mouse_x, double mouse_y) {
  m_selected.clear();
  for (unsigned int i = 0; i < m_tiles.size(); i++) {
    if (contains(m_tiles.at(i), mouse_x, mouse_y)) {
      m_tiles.at(i) = m_tiles.back();
      m_tiles.pop_back();
      return;
    }
  }
}

int game::get_n_ticks() const{
  return m_n_tick;
}

int game::get_agent_count(agent_type type){
    int count = 0;
    for (unsigned int i = 0; i<m_agents.size(); i++) {
        agent a = m_agents.at(i);
        if (a.get_type() == type){
            ++count;
        }
    }

    return count;
}

bool is_on_specific_tile(const double x, const double y, const tile& t)
{
  sf::Vector2f corner = t.get_corner();
  return x >= corner.x - 6 &&
         x <= corner.x + t.get_width() + 6 &&
         y >= corner.y - 6 &&
         y <= corner.y + t.get_height() + 6;
}

bool is_on_specific_tile(const agent& a, const tile& t) {
  sf::Vector2f center = a.get_center(sfml_resources::get().get_agent_sprite(a));
  return is_on_specific_tile(center.x, center.y, t);
}

bool is_on_tile(const game& g, const double x, const double y)
{
  for (tile t: g.get_tiles()) {
    sf::Vector2f corner = t.get_corner();
    if(x >= corner.x - 6 &&
       x <= corner.x + t.get_width() + 6 &&
       y >= corner.y - 6 &&
       y <= corner.y + t.get_height() + 6)
      return true;
  }
  return false;
}

std::vector<tile_type> get_on_tile_type(const game& g, const agent& a)
{
  const std::vector<tile> tiles = get_on_tile(g, a);
  if (tiles.empty()) return {};
  assert(tiles.size() == 1);
  return { tiles[0].get_type() };
}

std::vector<tile> get_on_tile(const game& g, const double x, const double y)
{
  for (tile t : g.get_tiles())
  {
    sf::Vector2f corner = t.get_corner();
    if (x >= corner.x - 6.0 &&
        x <= corner.x + t.get_width() + 6.0 &&
        y >= corner.y - 6.0 &&
        y <= corner.y + t.get_height() + 6.0
    )
    {
      return { t };
    }
  }
  return {};
}

std::vector<tile> get_on_tile(const game& g, const agent& a)
{
  sf::Vector2f center = a.get_center(sfml_resources::get().get_agent_sprite(a));
  return get_on_tile(g, center.x, center.y);
}

sf::Vector2f get_agent_center(const agent& a){
  return a.get_center(sfml_resources::get().get_agent_sprite(a));
}

double get_agent_width(const sf::Texture& a){
  return a.getSize().x;
}

double get_agent_height(const sf::Texture& a){
  return a.getSize().y;
}

bool is_on_tile(const game& g, const agent& a) {
  sf::Vector2f center = a.get_center(sfml_resources::get().get_agent_sprite(a));
  return is_on_tile(g, center.x, center.y);
}

std::vector<tile> get_current_tile(game& g, const agent& a){
  sf::Vector2f center = a.get_center(sfml_resources::get().get_agent_sprite(a));
  return get_current_tile(g, center.x, center.y);
}

std::vector<tile> get_current_tile(game& g, double x, double y)
{
  for(tile t: g.get_tiles())
  {
    if(is_on_specific_tile(x, y, t))
    {
      return { t };
    }
  }
  return {};
}

void game::confirm_tile_move(tile& t, int direction, int tile_speed){
  switch (direction)
  {
    // omhoog
    case 1:
      t.set_dy(-tile_speed);
      return;
    // rechts
    case 2:
      t.set_dx(tile_speed);
      return;
    // beneden
    case 3:
      t.set_dy(tile_speed);
      return;
    // links
    case 4:
      t.set_dx(-tile_speed);
      return;
    default:
      return;
  }
}

bool game::is_selected()
{
    return m_selected.empty();
}

void game::save_this(const std::string filename) const {
  save(*this, filename);
}

void test_game() //!OCLINT a testing function may be long
{
  // A game starts with one or more tiles
  {
    const game g;
    assert(!g.get_tiles().empty());
  }

  // A game starts with a score of zero
//  {
//    const game g;
//    assert(g.get_score() == 0);
//  }

  // A game starts with a zero number of game cycles
  {
    const game g;
    assert(g.get_n_ticks() == 0);
  }
  // Number of game cycles is increased each time all events are processed
  {
    game g;
    sound_type st { sound_type::none };
    g.process_events(st);
    assert(g.get_n_ticks() == 1);
  }

  // A game can be saved
  {
    const game g;
    const std::string filename{"tmp"};
    const QString actual_path = QString::fromStdString(SAVE_DIR) + filename.c_str() + ".sav";
    //if (QFile::exists(filename.c_str()))
    //{
      std::remove(filename.c_str());
    //}
    assert(!QFile::exists(filename.c_str()));
    g.save_this(filename);
    assert(QFile::exists(actual_path));
  //    assert(!get_saves().empty());
  }

  //'is_on_tile' should detect if there is a tile at a certain coordinat
  //positive control
  {
    //Tile at (0.0, 0.0, 0.0) with width and height of 10.0
    const std::vector<tile> tiles = { tile(0.0, 0.0, 0.0, 10.0, 10.0) };
    const std::vector<agent> no_agents;
    const game g(tiles, no_agents);
    //Coordinat (1.0, 1.0) is on a tile
    assert(is_on_tile(g, 1.0, 1.0));
    //Coordinat (-100.0, -100.0) is not on a tile
    assert(!is_on_tile(g, -100.0, -100.0));
  }
  {
    const game g(std::vector<tile>{tile(0, 0, 0, 0, 0, tile_type::grassland)},
                 std::vector<agent>{agent(agent_type::cow, 0, 0, 100)}
                );
    assert(g.get_agents().size() == 1);
    assert(g.get_tiles().size() == 1);
  }
  #define FIX_ISSUE_97
  #ifdef FIX_ISSUE_97
  // A game can be loaded
  {
    const game g(create_test_default_tiles(),
                 std::vector<agent>{agent(agent_type::spider, 0, 0, 100)}
                );
    const std::string filename{"tmp"};
    const QString actual_path = QString::fromStdString(SAVE_DIR) + filename.c_str() + ".sav";
    //if (QFile::exists(filename.c_str()))
    //{
      std::remove(filename.c_str());
    //}
    assert(!QFile::exists(filename.c_str()));
    save(g, filename);
    assert(QFile::exists(actual_path));
    game h = load(filename);
    assert(g == h);
  }
  #endif // FIX_ISSUE_97

  //Test confirm_tile_move up
  {
    const std::vector<agent> no_agents;
    game g( { tile(0.0, 0.0, 0.0, 10.0, 10.0)}, no_agents);
    sound_type st { sound_type::none };
    tile& tile = g.get_tiles()[0];

    g.confirm_tile_move(tile, 1, 1); // up

    g.process_events(st);

    assert(tile.get_x() == 0);
    assert(tile.get_y() == -1);
  }

  //Test remove_tile
  {
    const std::vector<agent> no_agents;
    game g( {
              tile(300.0, 300.0, 0.0, 10.0, 10.0),
              tile(0.0, 0.0, 0.0, 10.0, 10.0)
            }, no_agents);
    sound_type st { sound_type::none };

    g.remove_tile(0.0, 0.0);

    g.process_events(st);

    int tilecount = g.get_tiles().size();

    assert(tilecount == 1);
  }

  //Test confirm_tile_move left
  {
    const std::vector<agent> no_agents;
    game g( { tile(0.0, 0.0, 0.0, 10.0, 10.0)}, no_agents);
    sound_type st { sound_type::none };
    tile& tile = g.get_tiles()[0];

    g.confirm_tile_move(tile, 4, 1); // left

    g.process_events(st);

    g.get_tiles();
    assert(tile.get_x() == -1);
    assert(tile.get_y() == 0);

  }
  //Test confirm_tile_move down
  {
    const std::vector<agent> no_agents;
    game g( { tile(0.0, 0.0, 0.0, 10.0, 10.0)}, no_agents);
    sound_type st { sound_type::none };
    tile& tile = g.get_tiles()[0];

    g.confirm_tile_move(tile, 3, 1); // down

    g.process_events(st);

    assert(tile.get_x() == 0);
    assert(tile.get_y() == 1);

  }
  //Test confirm_tile_move right
  {
    const std::vector<agent> no_agents;
    game g( { tile(0.0, 0.0, 0.0, 10.0, 10.0)}, no_agents);
    sound_type st { sound_type::none };
    tile& tile = g.get_tiles()[0];

    g.confirm_tile_move(tile, 2, 1); // right

    g.process_events(st);

    assert(tile.get_x() == 1);
    assert(tile.get_y() == 0);

  }

  //#define FIX_ISSUE_617
  #ifdef FIX_ISSUE_617
  //Two grasses should merge to one mountain
  {
    // Create a game with two grassland blocks on top of each other
    // +====+====+    +----+----+
    // || grass || -> |mountain|
    // +====+====+    +----+----+
    const std::vector<tile> tiles
    {
      //   x    y    z    r    type         ID
      tile(0.0, 0.0, 0.0, 0.0, 0.0, tile_type::grassland, tile_id()),
      tile(0.0, 0.0, 0.0, 0.0, 0.0, tile_type::grassland, tile_id())
    };

    game g(tiles);
    sound_type st { sound_type::none };
    assert(count_n_tiles(g) == 2);
    assert(collect_tile_types(g)[0] == tile_type::grassland);
    assert(collect_tile_types(g)[1] == tile_type::grassland);
    g.process_events(st);
    assert(count_n_tiles(g) == 1);
    assert(collect_tile_types(g)[0] == tile_type::hills);
  }
  #endif
  //When an agent dies, score must decrease
  //Depends on #285
//  { //TODO rewrite this test
//    game g(create_default_tiles(), { agent(agent_type::cow) } );
//    assert(!g.get_agents().empty());
//    double prev_score = g.get_score();
//    // Wait until cow starves
//    while (!g.get_agents().empty())
//    {
//      sound_type st { sound_type::none };
//      g.process_events(st);
//    }
//    const double new_score = g.get_score();
//    assert(new_score < prev_score);
//  }
  //A game event should move tiles
  {
    const std::vector<agent> no_agents;
    game g( { tile(0.0, 0.0, 0.0, 10.0, 10.0) }, no_agents);
    sound_type st { sound_type::none };
    tile& tile = g.get_tiles()[0];
    const auto x_before = tile.get_x();
    const auto y_before = tile.get_y();
    tile.set_dx(5.0);
    tile.set_dy(5.0);
    g.process_events(st);
    const auto x_after = tile.get_x();
    const auto y_after = tile.get_y();
    assert(x_before != x_after);
    assert(y_before != y_after);
  }
  //#define FIX_ISSUE_415
  #ifdef FIX_ISSUE_415
  {
    // Create a game with two grassland blocks, one with a cow,
    // one with a grass agent
    //
    //     0             x               40
    //  +--+-------------|----------------+------
    //  |
    // 0+  +=============================+
    //  |  |grassland witC cow agent     |
    //10+  +=============================+
    //  |  | grassland wiGh grass agent  |
    //20+  +=============================+
    // The cow will move towards the grass and should cross the chasm
    // between the tiles
    game g(
      { //   x     y    z    w     h
        tile(0.0,  0.0, 0.0, 40.0, 10.0),
        tile(0.0, 10.0, 0.0, 40.0, 10.0)
      },
      {
        agent(agent_type::cow  , 20.0,  5.0),
        agent(agent_type::grass, 20.0, 15.0),
      }
    );
    //Will freeze
    sound_type st { sound_type::none };
    while (g.get_agents()[0].get_y() < 11.0) {
      g.process_events(st);
    }
  }
  #endif //
  //Agents must follow the movement of the tile they are on
  {
    //Put a cow on a grass tile, then move tile down and rightwards
    const double start_cow_x = 1.0;
    const double start_cow_y = 1.0;
    game g(
      { tile(0.0, 0.0, 0.0, 10.0, 10.0) },
      { agent(agent_type::cow, start_cow_x, start_cow_y) }
    );
    sound_type st { sound_type::none };
    tile& tile = g.get_tiles()[0];
    const auto x_before = tile.get_x();
    tile.set_dx(5.0);
    g.process_events(st);
    const auto x_after = tile.get_x();
    assert(x_before != x_after);
    tile.set_dy(5.0);
    g.process_events(st);
  }
  {
    const agent a(agent_type::tree);
    sf::Texture &sprite = sfml_resources::get().get_agent_sprite(a);
    assert(a.is_clicked(1,1,sprite) == true);
    assert(a.is_clicked(-100,-100,sprite) == false);
  }
//  //Agents must not be pushed off their tile, #317
//  {
//    //Put a grass agent on a grass tile,
//    //then move another tile on it
//    const double start_grass_x = 1.0;
//    const double start_grass_y = 1.0;
//    game g(
//      {
//        tile(-10.0, -10.0, 0.0, 10.0, 10.0), // Left tile that will move to right
//        tile(  0.0,   0.0, 0.0, 10.0, 10.0)  // Right tile with cow
//      },
//      { agent(agent_type::grass, start_grass_x, start_grass_y) }
//    );
//    sound_type st { sound_type::none };
//    tile& tile = g.get_tiles()[0];
//    tile.set_dx(1.0);
//    tile.set_dy(1.0);
//    for (int i=0; i != 100; ++i)
//    {
//      g.process_events(st);
//    }
//    assert(g.get_agents()[0].get_x() == start_grass_x);
//    assert(g.get_agents()[0].get_y() == start_grass_y);
//  }

  //Get agent count function test (Issue: #373)
    {
        game g(create_test_default_tiles(), { agent(agent_type::cow),
                                         agent(agent_type::cow),
                                         agent(agent_type::cow),
                                         agent(agent_type::cow),
                                         agent(agent_type::cow),
                                         agent(agent_type::plankton) } );
        // There are now 5 agents of type cow
        assert(g.get_agent_count(agent_type::cow) == 5);
    }
    //random_int() returns a random int between min (inclusive) and max (inclusive)
    {
      int random = random_int(0, 10);
      assert(random >= 0 && random <= 10);
    }
    //random_double() returns a random double between min (inclusive) and max (inclusive)
    {
      double random = random_double(0.0, 10.0);
      assert(random >= 0.0 && random <= 10.0);
    }
    //test confirm_tile_move()
    {
      game g;
      tile t(0, 0, 0, 0, 0, tile_type::grassland);
      g.confirm_tile_move(t, 1, 1);
      assert(t.get_dy() == -1);
      g.confirm_tile_move(t, 2, 1);
      assert(t.get_dx() == 1);
      g.confirm_tile_move(t, 3, 1);
      assert(t.get_dy() == 1);
      g.confirm_tile_move(t, 4, 1);
      assert(t.get_dx() == -1);
  }
  {
    game g;
    g.allow_spawning();
    g.allow_damage();
    g.allow_score();
  }
  {
    std::clog << "saves:" << std::endl;
    for (auto &a : get_saves()) {
      std::clog << a << "\n";
    }
    std::clog << std::endl;
  }
  {
    // Test "game::check_selection"
        game g({tile(0, 0, 0, 0, 0, tile_type::grassland)});
        assert(g.is_selected());
        g.check_selection(0, 0);
        assert(!g.is_selected());
        g.check_selection(500, 500);
        assert(g.is_selected());
  }

  // Operator== tests
  //#define FIX_ISSUE_583
  #ifdef FIX_ISSUE_583
  { // Two default-constructed games should be the same
    game g1;
    game g2;
    assert(g1 == g2);
  }
  // Two default-constructed games, after a change to one game, should result in two different games
  { // tick/score/essence change
    game g1;
    game g2;
    sound_type st = sound_type::none;
    for (int i = 0; i < 50; i++)
      g1.process_events(st);
    assert(!(g1 == g2));
  }
  { // agent change
    game g1;
    game g2;
    agent a(agent_type::bird);
    g1.add_agents({a});
    assert(!(g1 == g2));
  }
  { // tile difference
    game g1;
    tile t;
    game g2({t});
    assert(!(g1 == g2));
  }
  #endif
}

game load(const std::string &filename) {
  std::ifstream f(SAVE_DIR + filename + ".sav");
  game g;
  f >> g;
  return g;
}

void save(const game &g, const std::string &filename) {
  QString path = QDir::currentPath() + QString::fromStdString(SAVE_DIR);
  QDir dir = QDir::root();
  dir.mkpath(path);

  std::ofstream f(SAVE_DIR + filename + ".sav");
  f << g;
}

std::vector<std::string> get_saves() {
  QDir dir(QDir::currentPath() + "/" + QString::fromStdString(SAVE_DIR));
  std::vector<std::string> filenames{};
  std::list<QString> entries = dir.entryList().toStdList();
  for (QString qstr : entries) {
    std::string str = qstr.toStdString();
    if (str.size() > 4 &&
        str.substr(str.size() - 4, 4) == ".sav") {
      str.erase(str.size() - 4, 4);
      filenames.push_back(str);
    }
  }
  return filenames;
}

std::ostream& operator<<(std::ostream& os, const game& g)
{
  os << g.m_n_tick << ' ' << g.m_score << ' ' << g.m_essence << ' '
     << g.m_tiles.size() << ' '
     << g.m_agents.size();
  for (int i=0; i < static_cast<int>(g.m_tiles.size()); i++){
      os << ' ' << g.m_tiles[i];
  }
  for (int i=0; i < static_cast<int>(g.m_agents.size()); i++){
      os << ' ' << g.m_agents[i];
  }

  os << ' ';

  return os;
}

std::istream& operator>>(std::istream& is, game& g)
{
  is >> g.m_n_tick >> g.m_score >> g.m_essence;
  int n_tiles = 0;
  is >> n_tiles;
  int n_agents = 0;
  is >> n_agents;
  g.m_tiles.clear();
  for (int i = 0; i < n_tiles; ++i)
  {
    tile t(112, 112, 112, 0, 0, tile_type::grassland);
    is >> t;
    g.m_tiles.emplace_back(t);
  }
  g.m_agents.clear();
  for (int i = 0; i < n_agents; ++i)
  {
    agent a(agent_type::cow, 0, 0, 1);
    is >> a;
    g.m_agents.emplace_back(a);
  }
  return is;
}

bool operator==(const game& lhs, const game& rhs) noexcept
{
  return lhs.m_n_tick == rhs.m_n_tick &&
         lhs.m_score == rhs.m_score &&
         lhs.m_essence == rhs.m_essence &&
         lhs.m_tiles == rhs.m_tiles &&
         lhs.m_agents == rhs.m_agents;
}
