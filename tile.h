#ifndef TILE_H
#define TILE_H

#include "tile_type.h"

///A world tile
class tile
{
public:
  ///Constructor
  /// @param x the x-coordinat of the top-left corner of the tile
  /// @param y the y-coordinat of the top-left corner of the tile
  /// @param width the width of the tile
  /// @param height the height of the tile
  /// @param type the type the tile
  tile(
    const double x,
    const double y,
    const double width,
    const double height,
    const tile_type type
  );

private:

  /// The height of the tile
  const double m_height;

  /// The type the tile
  const tile_type m_type;

  /// The width of the tile
  const double m_width;

  /// The x-coordinat of the top-left corner of the tile
  double m_x;

  /// The y-coordinat of the top-left corner of the tile
  double m_y;

};

#endif // TILE_H
