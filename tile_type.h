#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include <iostream>
/// The type a tile can be
enum class tile_type {
  none,
  nonetile,
  grassland,
  mountains,
  ocean,
  savannah,
  arctic,
  desert
};

std::ostream& operator<<(std::ostream& os, const tile_type t) noexcept;
std::istream& operator>>(std::istream& is, tile_type& t) noexcept;

#endif // TILE_TYPE_H
