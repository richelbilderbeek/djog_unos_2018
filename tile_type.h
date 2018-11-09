#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include <vector>
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
  desert,
  swamp
};

///Get the merged type of two type names,
///e.g.
tile_type get_merge_type(tile_type type1, tile_type type2) noexcept;

///Test the tile_type function
void test_tile_type();

std::vector<tile_type> get_all_tile_types() noexcept;
///Get all tile types

std::ostream& operator<<(std::ostream& os, const tile_type t) noexcept;
std::istream& operator>>(std::istream& is, tile_type& t) noexcept;

#endif // TILE_TYPE_H
