#include "tile.h"

#include <cassert>

tile::tile(
  const double x,
  const double y,
  const double width,
  const double height,
  const tile_type type
) : m_height{height},
    m_type{type},
    m_width{width},
    m_x{x},
    m_y{y}
{
  assert(m_width > 0.0);
  assert(m_height > 0.0);
}
