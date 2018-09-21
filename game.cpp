//Always include the header of the unit first
#include "game.h"

#include <cassert>

game::game()
  : m_tiles{}
{
  //Add first tile
  {
    tile t(200, 200, 400, 200, tile_type::grassland);
    m_tiles.push_back(t);
  }
}

void test_game()
{
  //A game starts with one or more tiles
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
}
