#include "tile_type.h"
#include "string"
#include <cassert>
#include <iostream>
#include <sstream>

std::vector<tile_type> collect_all_tile_types()
{
  return
  {
        tile_type::none,
        tile_type::grassland,
        tile_type::mountains,
        tile_type::ocean,
        tile_type::savannah,
        tile_type::arctic,
        tile_type::desert
  };
}

void test_tile_type() //!OCLINT testing function may be many lines
{
  // Stream in and out must be symmetrical
  {
    for (const tile_type t: collect_all_tile_types())
    {
      std::stringstream s;
      s << t;
      tile_type u;
      s >> u;
      assert(t == u);
    }
  }
}

std::string to_str(tile_type t)
{
  switch (t) {
    case tile_type::grassland:
        return "grassland";
      break;

    case tile_type::arctic:
        return "arctic";
      break;

    case tile_type::desert:
        return "desert";
    break;

    case tile_type::mountains:
        return "mountains";
    break;

    case tile_type::ocean:
        return "ocean";
      break;

    case tile_type::savannah:
        return "savannah";
      break;

    default:
      return "none";
      break;
  }
}

tile_type to_tile(std::string str)
{
  if (str == "arctic") return tile_type::arctic;
  if (str == "grassland") return tile_type::grassland;
  if (str == "desert") return tile_type::desert;
  if (str == "mountains") return tile_type::mountains;
  if (str == "ocean") return tile_type::ocean;
  if (str == "savannah") return tile_type::savannah;
  return tile_type::none;
}

std::ostream& operator <<(std::ostream& os, const tile_type t) noexcept {
    os << to_str(t);
    return os;
}

std::istream& operator>>(std::istream& is, tile_type& t) noexcept {
    std::string str;
    is >> str;
    t = to_tile(str);
    return is;
}
