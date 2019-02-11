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


game::game(const std::vector<tile>& tiles,
           const std::vector<agent>& agents,
           const int starting_tick)
  : m_tiles{tiles},
    m_agents{agents},
    m_n_tick{starting_tick},
    m_score{0}
{

}

void game::add_agents(const std::vector<agent>& as)
{
  std::copy(
    std::begin(as),
    std::end(as),
    std::back_inserter(m_agents)
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

int count_n_tiles(const game& g) noexcept
{
  return g.get_tiles().size();
}

int count_n_agents(const game& g) noexcept
{
  return g.get_agents().size();
}

void game::process_events()
{
  for (auto& a: m_agents) {
    a.process_events(*this);
  }

  kill_agents();

  merge_tiles();

  //Process the events happening on the tiles
  for (auto& tile: m_tiles)
  {
    if(tile.get_dx() != 0 || tile.get_dy() != 0){
      tile.move();
    }
    tile.process_events();
  }

  // DO NOT DO FOR AGENT IN GET_AGENTS HERE

  ++m_n_tick;
}

void game::tile_merge(tile& focal_tile, const tile& other_tile, const int other_pos) {
  // Merge attempt with this function
  const tile_type merged_type = get_merge_type(
    focal_tile.get_type(),
    other_tile.get_type()
  );
  //focal tile becomes merged type
  focal_tile.set_type(merged_type);
  //other tile is swapped to the back, then deleted
  m_tiles[other_pos] = m_tiles.back();
  m_tiles.pop_back();
  //change the selected tile
  m_selected.clear();
  assert(m_selected.empty());
}

void game::move_tiles(sf::RenderWindow& window, sfml_camera& camera){
  bool clicked_tile = false;
  for (unsigned i = 0; i < m_tiles.size(); i++)
  {
    if (contains(m_tiles.at(i),
          sf::Mouse::getPosition(window).x + camera.x,
          sf::Mouse::getPosition(window).y + camera.y))
    {
      for (unsigned j = 0; j < m_tiles.size(); j++)
      {
        if (m_tiles.at(j).get_dx() != 0 || m_tiles.at(j).get_dy() != 0){
          return;
        }
      }
      tile s_tile = m_tiles.at(i);
      m_selected.clear();
      m_selected.push_back(s_tile.get_id());
      clicked_tile = true;
    }
  }
  if (clicked_tile == false)
  {
    for (unsigned i = 0; i < m_tiles.size(); i++)
    {
      if (m_tiles.at(i).get_dx() != 0 || m_tiles.at(i).get_dy() != 0){
        return;
      }
    }
    m_selected.clear();
  }
}

void game::merge_tiles() { //!OCLINT must simplify
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
      if (!have_same_position(focal_tile, other_tile)) { return; }
      tile_merge(focal_tile, other_tile, j);
      focal_tile.set_dx(0);
      focal_tile.set_dy(0);
      other_tile.set_dx(0);
      other_tile.set_dy(0);
      for(agent& a: m_agents){
        if(is_on_specific_tile(a, focal_tile)){
          a.set_direction(NAN);
        }
      }
      for(agent& a: m_agents){
        if(is_on_specific_tile(a, other_tile)){
          a.set_direction(NAN);
        }
      }
    }
  }
}

void game::kill_agents() {
  const int n = count_n_agents(*this);
  for (int i = 0; i < n; ++i) {
    if (m_agents[i].get_health() <= 0) {
      m_agents[i] = m_agents.back();
      m_agents.pop_back();
      --m_score;
    }
  }
}

void game::remove_tile(sf::RenderWindow& window, sfml_camera& camera) {
  for (unsigned i = 0; i < m_tiles.size(); ++i) {
    if (contains(m_tiles.at(i),
       sf::Mouse::getPosition(window).x + camera.x,
       sf::Mouse::getPosition(window).y + camera.y))
    {
       if(m_tiles[i].get_id() == m_selected[0]){
          m_selected.pop_back();
       }
       m_tiles[i] = m_tiles.back();
       m_tiles.pop_back();
    }
  }
}

int game::get_n_ticks() const{
  return m_n_tick;
}

bool is_on_specific_tile(const double x, const double y, const tile& t)
{
  return x >= t.get_x() - 5 &&
         x <= t.get_x() + t.get_width() + 5 &&
         y >= t.get_y() - 5 &&
         y <= t.get_y() + t.get_height() + 5;
}

bool is_on_specific_tile(const agent& a, const tile& t) {
  sf::Vector2f center = a.get_center(sfml_resources::get().get_agent_sprite(a));
  return is_on_specific_tile(center.x, center.y, t);
}

bool is_on_tile(const game& g, const double x, const double y)
{
  for (tile t: g.get_tiles()){
    if(x >= t.get_x() - 5 &&
       x <= t.get_x() + t.get_width() + 5 &&
       y >= t.get_y() - 5 &&
       y <= t.get_y() + t.get_height() + 5)
      return true;
  }
  return false;
}

tile_type get_on_tile_type(const game& g, const agent& a)
{
  for (tile t: g.get_tiles()){
    if(a.get_x() >= t.get_x() - 5 &&
       a.get_x() <= t.get_x() + t.get_width() + 5 &&
       a.get_y() >= t.get_y() - 5 &&
       a.get_y() <= t.get_y() + t.get_height() + 5)
      return t.get_type();
  }
  return tile_type::nonetile;
}

bool is_on_tile(const game& g, const agent& a) {
  sf::Vector2f center = a.get_center(sfml_resources::get().get_agent_sprite(a));
  return is_on_tile(g, center.x, center.y);
}

void game::confirm_tile_move(tile& t, int direction, int tile_speed){
  switch (direction)
  {
    case 1:
      t.set_dy(-tile_speed);
      for(agent& a: m_agents){
        if(is_on_specific_tile(a, t)){
          a.set_direction(180);
        }
      }
      return;
    case 2:
      t.set_dx(tile_speed);
      for(agent& a: m_agents){
        if(is_on_specific_tile(a, t)){
          a.set_direction(90);
        }
      }
      return;
    case 3:
      t.set_dy(tile_speed);
      for(agent& a: m_agents){
        if(is_on_specific_tile(a, t)){
          a.set_direction(0);
        }
      }
      return;
    case 4:
      t.set_dx(-tile_speed);
      for(agent& a: m_agents){
        if(is_on_specific_tile(a, t)){
          a.set_direction(270);
        }
      }
      return;
    default:
      return;
  }
}

int count(const game& g, const agent_type t){
  int count = 0;
  for(agent a: g.get_agents()){
    if(a.get_type() == t){
      count++;
    }
  }
  return count;
}

void test_game() //!OCLINT a testing function may be long
{
  // A game starts with one or more tiles
  {
    const game g;
    assert(!g.get_tiles().empty());
  }

  // A game starts with a score of zero
  {
    const game g;
    assert(g.get_score() == 0);
  }

  // A game starts with a zero number of game cycles
  {
    const game g;
    assert(g.get_n_ticks() == 0);
  }
  // Number of game cycles is increased each time all events are processed
  {
    game g;
    g.process_events();
    assert(g.get_n_ticks() == 1);
  }

  // A game can be saved
  {
    const game g;
    const std::string filename{"tmp.sav"};
    if (QFile::exists(filename.c_str()))
    {
      std::remove(filename.c_str());
    }
    assert(!QFile::exists(filename.c_str()));
    save(g, filename);
    assert(QFile::exists(filename.c_str()));
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
    const game g(std::vector<tile>{tile(0, 0, 0, 1, 1, 0, tile_type::grassland)},
                 std::vector<agent>{agent(agent_type::cow, 0, 0, 100)}
                );
    assert(g.get_agents().size() == 1);
    assert(g.get_tiles().size() == 1);
  }
  // A game can be loaded
  {
    const game g(create_default_tiles(),
                 std::vector<agent>{agent(agent_type::spider, 0, 0, 100)}
                );
    const std::string filename{"tmp.sav"};
    if (QFile::exists(filename.c_str()))
    {
      std::remove(filename.c_str());
    }
    assert(!QFile::exists(filename.c_str()));
    save(g, filename);
    assert(QFile::exists(filename.c_str()));
    const game h = load(filename);
    assert(g == h);
  }
  //Two grasses should merge to one mountain
  {
    // Create a game with two grassland blocks on top of each other
    // +====+====+    +----+----+
    // || grass || -> |mountains|
    // +====+====+    +----+----+
    const std::vector<tile> tiles
    {
      //   x    y    z   w    h    type         ID
      tile(1.0, 1.0, 1.0, 2.0, 1.0, 0.0, tile_type::grassland, tile_id()),
      tile(1.0, 1.0, 1.0, 2.0, 1.0, 0.0, tile_type::grassland, tile_id())
    };

    game g(tiles);
    assert(count_n_tiles(g) == 2);
    assert(collect_tile_types(g)[0] == tile_type::grassland);
    assert(collect_tile_types(g)[1] == tile_type::grassland);
    g.process_events();
    assert(count_n_tiles(g) == 1);
    assert(collect_tile_types(g)[0] == tile_type::hills);
  }
  //When an agent dies, score must decrease
  //Depends on #285
  {
    game g(create_default_tiles(), { agent(agent_type::cow) } );
    assert(!g.get_agents().empty());
    double prev_score = g.get_score();
    // Wait until cow starves
    while (!g.get_agents().empty())
    {
      g.process_events();
    }
    const double new_score = g.get_score();
    assert(new_score < prev_score);
  }
  //A game event should move tiles
  {
    const std::vector<agent> no_agents;
    game g( { tile(0.0, 0.0, 0.0, 10.0, 10.0) }, no_agents);
    tile& tile = g.get_tiles()[0];
    const auto x_before = tile.get_x();
    const auto y_before = tile.get_y();
    tile.set_dx(5.0);
    tile.set_dy(5.0);
    g.process_events();
    const auto x_after = tile.get_x();
    const auto y_after = tile.get_y();
    assert(x_before != x_after);
    assert(y_before != y_after);
  }
  //Agents must follow the movement of the tile they are on
  {
    //Put a cow on a grass tile, then move tile down and rightwards
    const double start_cow_x = 1.0;
    const double start_cow_y = 1.0;
    game g(
      { tile(0.0, 0.0, 0.0, 10.0, 10.0) },
      { agent(agent_type::cow, start_cow_x, start_cow_y) }
    );
    tile& tile = g.get_tiles()[0];
    agent& agent = g.get_agents()[0];
    const auto x_before = tile.get_x();
    tile.set_dx(5.0);
    agent.set_direction(90);
    g.process_events();
    const auto x_after = tile.get_x();
    assert(x_before != x_after);
    tile.set_dy(5.0);
    agent.set_direction(0);
    g.process_events();
    assert(g.get_agents()[0].get_x() > start_cow_x);
    assert(g.get_agents()[0].get_y() > start_cow_y);
  }
  {
    const agent a(agent_type::tree);
    sf::Texture &sprite = sfml_resources::get().get_agent_sprite(a);
    assert(a.is_clicked(1,1,sprite) == true);
    assert(a.is_clicked(-100,-100,sprite) == false);
  }
  //Agents must not be pushed off their tile, #317
  {
    //Put a grass agent on a grass tile,
    //then move another tile on it
    const double start_grass_x = 1.0;
    const double start_grass_y = 1.0;
    game g(
      {
        tile(-10.0, -10.0, 0.0, 10.0, 10.0), // Left tile that will move to right
        tile(  0.0,   0.0, 0.0, 10.0, 10.0)  // Right tile with cow
      },
      { agent(agent_type::grass, start_grass_x, start_grass_y) }
    );
    tile& tile = g.get_tiles()[0];
    tile.set_dx(1.0);
    tile.set_dy(1.0);
    for (int i=0; i != 100; ++i)
    {
      g.process_events();
    }
    assert(g.get_agents()[0].get_x() == start_grass_x);
    assert(g.get_agents()[0].get_y() == start_grass_y);
  }
  //Count the agents of a type
  {
    game g(create_default_tiles(),
           {agent(agent_type::cow),
           agent(agent_type::cow),
           agent(agent_type::grass)});
    int cows = count(g, agent_type::cow);
    assert(cows == 2);
    int grass = count(g, agent_type::grass);
    assert(grass == 1);
  }
}

game load(const std::string &filename) {
  std::ifstream f(filename);
  game g;
  f >> g;
  return g;
}

void save(const game &g, const std::string &filename) {
  std::ofstream f(filename);
  f << g;
}

std::ostream& operator<<(std::ostream& os, const game& g)
{
  os << g.m_n_tick << ' ' << g.m_score << ' '
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
  is >> g.m_n_tick >> g.m_score;
  int n_tiles = 0;
  is >> n_tiles;
  int n_agents = 0;
  is >> n_agents;
  g.m_tiles.clear();
  for (int i = 0; i < n_tiles; ++i)
  {
    tile t(1, 1, 1, 1, 1, 0, tile_type::grassland, tile_id());
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
         lhs.m_tiles == rhs.m_tiles &&
         lhs.m_agents == rhs.m_agents;
}
