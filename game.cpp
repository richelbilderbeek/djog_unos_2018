//Always include the header of the unit first
#include <game.h>

game::game()
  : m_tiles{}
{
  //Add first tile
  {
    tile t(200, 200, 400, 200, tile_type::grassland);
    m_tiles.push_back(t);
  }
}

