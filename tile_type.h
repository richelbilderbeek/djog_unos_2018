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

///Get the merged type of two type names,
///e.g.
tile_type get_merge_type(tile_type type1, tile_type type2) noexcept;

///Test the tile_type function
void test_tile_type();

std::ostream& operator<<(std::ostream& os, const tile_type t) noexcept;
std::istream& operator>>(std::istream& is, tile_type& t) noexcept;

#endif // TILE_TYPE_H
