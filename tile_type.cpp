#include "tile_type.h"
#include "string"

#include <iostream>
std::string to_str(tile_type t)
{
  switch (t) {
    case tile_type::grassland:
      return "grassland";

    case tile_type::arctic:
      return "arctic";

    case tile_type::desert:
      return "desert";

    case tile_type::mountains:
      return "mountains";

    case tile_type::ocean:
      return "ocean";

    case tile_type::savannah:
      return "savannah";

    case tile_type::swamp:
      return "swamp";

    default:
      return "none";
  }
}

tile_type to_tile(std::string str)
{
  if (str == "arctic") return tile_type::arctic;
  if (str == "grassland") return tile_type::grassland;
  if (str == "desert") return tile_type::desert;
  if (str == "swamp") return tile_type::swamp;
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
    t = to_tile(str); //!OCLINT indeed parameter reassignment, which is exactly the idea
    return is;
}
