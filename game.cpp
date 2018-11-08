
// Always include the header of the unit first
#include "game.h"
#include <cassert>
#include <fstream>
#include <cstdio>
#include <QFile>

game::game() : m_tiles{}, m_score{0} {
  // Add first tile
  {
    tile t(115, 115, 215, 100, tile_type::grassland, new_id());
    m_tiles.push_back(t);
  }
  {
    tile t(0, 345, 215, 100, tile_type::grassland, new_id());
    m_tiles.push_back(t);
  }
  {
    tile t(230, 230, 100, 215, tile_type::mountains, new_id());
    m_tiles.push_back(t);
  }
  {
    tile t(0, 230, 215, 100, tile_type::ocean, new_id());
    m_tiles.push_back(t);
  }
  {
    tile t(0, 0, 215, 100, tile_type::arctic, new_id());
    m_tiles.push_back(t);
  }
  {
    tile t(230, 0, 215, 100, tile_type::savannah, new_id());
    m_tiles.push_back(t);
  }
  {
    tile t(345, 115, 100, 215, tile_type::swamp, new_id());
    m_tiles.push_back(t);
  }
  {
    tile t(460, 0, 100, 215, tile_type::desert, new_id());
    m_tiles.push_back(t);
  }
}

void game::add_tiles(std::vector<tile> ts) {
  for (tile& t : ts) {
    m_tiles.push_back(t);
  }
}

void game::delete_tiles(std::vector<tile> ts) {
  for (tile& t : ts) {
    auto here = std::find_if(
          std::begin(m_tiles),
          std::end(m_tiles),
          [t](const tile& u)
          {
            return u.get_id() == t.get_id();
          }
    );
    std::swap(*here, m_tiles.back());
    m_tiles.pop_back();
  }
}

void game::process_events() {
  for (auto& tile: m_tiles)
  {
    tile.process_events();
  }
  ++m_n_tick;
}

void test_game() //!OCLINT a testing function may be long
{
  // A game starts with one or more tiles
  {
    const game g;
    assert(!g.get_tiles().empty());
  }
//#define FIX_ISSUE_89_ADD_SECOND_TILE
#ifdef FIX_ISSUE_89_ADD_SECOND_TILE
  {
    const game g;
    assert(!g.get_tiles().size() >= 2);
    assert(g.get_tiles()[0].get_type() != g.get_tiles()[1].get_type());
  }
#endif // FIX_ISSUE_89_ADD_SECOND_TILE

  // A game starts with a score of zero
  {
    const game g;
    assert(g.get_score() == 0);
  }

//#define FIX_ISSUE_91_GAME_TRACKS_THE_NUMBER_OF_TICKS
#ifdef FIX_ISSUE_91_GAME_TRACKS_THE_NUMBER_OF_TICKS
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
#endif // FIX_ISSUE_91_GAME_TRACKS_THE_NUMBER_OF_TICKS

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
  // A game can be loaded
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
    const game h = load(filename);
    assert(g.get_score() == h.get_score());
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
     << g.m_tiles.size() << ' ';

  for (int i=0; i < static_cast<int>(g.m_tiles.size()); i++){
      os << g.m_tiles[i];
  }

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
      tile t(1, 1, 1, 1, tile_type::grassland, g.new_id());
      is >> t;
      g.m_tiles.emplace_back(t);
  }
  return is;
}
