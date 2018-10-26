#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include <vector>
#include <iostream>
/// The type a tile can be
enum class tile_type { none, grassland, mountains, ocean, savannah, arctic, desert };

std::ostream& operator<<(std::ostream& os, const tile_type t) noexcept;
std::istream& operator>>(std::istream& is, tile_type& t) noexcept;

void test_tile_type();

std::vector<tile_type> collect_all_tile_types();

#endif // TILE_TYPE_H
