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

  /// The height of the tile
  double get_height() const noexcept { return m_height; }

  /// The type the tile
  tile_type get_type() const noexcept { return m_type; }

  /// The width of the tile
  double get_width() const noexcept { return m_width; }

  /// The x-coordinat of the top-left corner of the tile
  double get_x() const noexcept { return m_x; }

  /// The y-coordinat of the top-left corner of the tile
  double get_y() const noexcept { return m_y; }

  /// The movement coeficient on the x-axis
  double get_dx() const noexcept {return m_dx; }

  /// The movement coeficient on the y-axis
  double get_dy() const noexcept {return m_dy; }

  /// Set the movement coeficient on the x-axis
  void set_dx(double dx);

  /// Set the movement coeficient on the y-axis
  void set_dy(double dy);

  /// Move the tile by the movement coeficients
  void move();

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

  /// The movement coeficient on the x-axis
  double m_dx;

  /// The movement coeficient on the y-axis
  double m_dy;
};

///Test the tile class
void test_tile();

#endif // TILE_H
