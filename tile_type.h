#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include <vector>
#include <iostream>
/// The type a tile can be
enum class tile_type {
  nonetile,
  grassland,
  mountains,
  water,
  savannah,
  arctic,
  desert,
  swamp,
  woods,
  beach
};

///Get the merged type of two type names,
///e.g.
tile_type get_merge_type(tile_type type1, tile_type type2) noexcept;

///Test the tile_type function
void test_tile_type();

std::string to_str(tile_type t);
tile_type to_tile(std::string str);

///Get all tile types
std::vector<tile_type> get_all_tile_types() noexcept;

std::ostream& operator<<(std::ostream& os, const tile_type t) noexcept;
std::istream& operator>>(std::istream& is, tile_type& t) noexcept;

void test_tile_type();

#endif // TILE_TYPE_H
