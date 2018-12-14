
// Always include the header of the unit first
#include "id.h"
#include "game.h"
#include <cassert>
#include <fstream>
#include <cstdio>
#include <QFile>
#include <algorithm>

game::game(const std::vector<tile>& tiles,
           const std::vector<agent>& agents)
  : m_tiles{tiles},
    m_agents{agents},
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
    a.move();
  }
  merge_tiles();
  //Process the events happening on the tiles
  for (auto& tile: m_tiles)
  {
    tile.process_events();
  }
  ++m_n_tick;
}

void game::merge_tiles() { //!OCLINT must simplify
  // I use indices here, so it is more beginner-friendly
  // one day, we'll use iterators
  const int n = count_n_tiles(*this);
  for (int i = 0; i < n; ++i)
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
      if (!have_same_position(focal_tile, other_tile)) return;
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
      return; //!OCLINT early return the only good option?
    }
  }
}

int game::get_n_ticks() const{
  return m_n_tick;
}

bool is_on_tile(const game& , const double , const double )
{
  return true; //STUB
}


bool is_on_tile(const game& g, const agent& a){
  if ( std::find(g.get_agents().begin(), g.get_agents().end(), a) != g.get_agents().end() ){
    double x = a.get_x();
    double y = a.get_y();
    return is_on_tile(g, x, y);
  }
  return false;
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

  #define FIX_ISSUE_261
  #ifdef FIX_ISSUE_261
  {
    const game g;
    agent a(agent_type::cow, 0, 0);
    assert(is_on_tile(g, a));
    //assert(!is_on_tile_t(g, a));
  }
  #endif

  //#define FIX_ISSUE_RAFAYEL
  #ifdef FIX_ISSUE_RAFAYEL
  // A game can be loaded
  {
    const game g(create_two_grass_tiles());
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
  #endif // FIX_ISSUE_RAFAYEL
  {
    // Create a game with two grassland blocks on top of each other
    // +====+====+    +----+----+
    // || grass || -> |mountains|
    // +====+====+    +----+----+
    const std::vector<tile> tiles
    {
      //   x    y    z   w    h    type         ID
      tile(1, 1, 1, 2, 1, tile_type::grassland, 0),
      tile(1, 1, 1, 2, 1, tile_type::grassland, 0)
    };

    game g(tiles);
    assert(count_n_tiles(g) == 2);
    assert(collect_tile_types(g)[0] == tile_type::grassland);
    assert(collect_tile_types(g)[1] == tile_type::grassland);
    g.process_events();
    assert(count_n_tiles(g) == 1);
    assert(collect_tile_types(g)[0] == tile_type::mountains);
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
  //TODO: actually save the tile and agents
  os << g.m_n_tick << ' ' << g.m_score << ' '
     << g.m_tiles.size();

  for (int i=0; i < static_cast<int>(g.m_tiles.size()); i++){
      os << ' ' <<g.m_tiles[i];
  }

  os << ' ';

  return os;
}

std::istream& operator>>(std::istream& is, game& g)
{
  //TODO: actually save the tile and agents
  is >> g.m_n_tick >> g.m_score;
  int n_tiles = 0;
  is >> n_tiles;
  //TODO: the line below is a stub
  for (int i=0; i!=n_tiles; ++i)
  {
      tile t(1, 1, 1, 1, 1, tile_type::grassland, new_id());
      is >> t;
      g.m_tiles.emplace_back(t);
  }
  return is;
}

bool operator==(const game& lhs, const game& rhs) noexcept
{
  if (lhs.m_n_tick != rhs.m_n_tick)
      return false;
  if (lhs.m_score != rhs.m_score)
      return false;
  if (lhs.m_tiles != rhs.m_tiles)
      return false;

  return true;
}
