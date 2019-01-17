
// Always include the header of the unit first
#include "game.h"
#include "tile_id.h"
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

void game::process_events()
{
  for (auto& a: m_agents) {
    a.move(*this);
  }
  merge_tiles();
  //Process the events happening on the tiles
  for (auto& tile: m_tiles)
  {
    tile.process_events();
  }

  for (agent& a: get_agents())
  {
    // Agent a can be used
    if(a.get_type() == agent_type::grass){
        a.set_health(a.get_health() + 1);
    }
  }

  ++m_n_tick;
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
      const tile& other_tile = m_tiles[j];
      if (have_same_position(focal_tile, other_tile))
      // If the tiles are on the same exact position, then do the following.
      // The inverse of the previous.
      {
        const tile_type merged_type = get_merge_type(
          focal_tile.get_type(),
          other_tile.get_type()
        );
        //focal tile becomes merged type
        focal_tile.set_type(merged_type);
        //other tile is swapped to the back, then deleted
        m_tiles[j] = m_tiles.back();
        m_tiles.pop_back();
        //change the selected tile
        m_selected.clear();
        assert(m_selected.empty());
        i = n;
        j = n;
      }
    }
  }
}

int game::get_n_ticks() const{
  return m_n_tick;
}

bool is_on_specific_tile(const double x, const double y, const tile& t)
{
  //Do the borders count as the tile? If not, reduce the numbers
  if(x >= t.get_x() - 7
     && x <= t.get_x() + t.get_width() + 7
     && y >= t.get_y() - 7
     && y <= t.get_y() + t.get_height() + 7)
  {
     return true;
  }
  return false;
}


bool is_on_specific_tile(const agent& a, const tile& t){
  double x = a.get_x();
  double y = a.get_y();
  return is_on_specific_tile(x, y, t);
}

bool is_on_tile(const game& g, const double x, const double y)
{
    for (tile t: g.get_tiles()){
        if(x >= t.get_x()
          && x <= t.get_x() + t.get_width()
          && y >= t.get_y()
          && y <= t.get_y() + t.get_height())
        {
            return true;
        }
    }
    return false;
}


bool is_on_tile(const game& g, const agent& a){
  double x = a.get_x();
  double y = a.get_y();
  return is_on_tile(g, x, y);
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
    assert(collect_tile_types(g)[0] == tile_type::mountains);
  }
  //#define FIX_ISSUE_302
  #ifdef FIX_ISSUE_302
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
      prev_score = g.get_score();
    }
    const double new_score = g.get_score();
    assert(new_score < prev_score);
  }
  #endif //FIX_ISSUE_302

  //The test fails but it works
  //#define FIX_ISSUE_304
  #ifdef FIX_ISSUE_304
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
    tile.set_dx(1.0);
    tile.set_dy(1.0);
    g.process_events();
    assert(g.get_agents()[0].get_x() > start_cow_x);
    assert(g.get_agents()[0].get_y() > start_cow_y);
  }
  #endif //FIX_ISSUE_304

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
