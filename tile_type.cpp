#include "tile_type.h"
#include "string"
#include <cassert>
#include <iostream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <string>

tile_type get_merge_type(tile_type type1, tile_type type2) noexcept //!OCLINT must be simpler
{
  if (type1 == tile_type::grassland && type2 == tile_type::grassland)
  {
    return tile_type::hills;
  }
  else if (
       (type1 == tile_type::grassland && type2 == tile_type::desert)
    || (type1 == tile_type::desert && type2 == tile_type::grassland)
  )
  {
    return tile_type::savannah;
  }
  else if (
       (type1 == tile_type::grassland && type2 == tile_type::water)
    || (type1 == tile_type::water && type2 == tile_type::grassland)
  )
  {
    return tile_type::swamp;
  }
  else if (
       (type1 == tile_type::mountains&& type2 == tile_type::water)
    || (type1 == tile_type::water && type2 == tile_type::mountains)
  )
  {
    return tile_type::arctic;
  }
  return tile_type::nonetile;
}

void test_tile_type()
{
  {
    // merging of types
    assert(get_merge_type(tile_type::nonetile, tile_type::grassland) == tile_type::nonetile);
    assert(get_merge_type(tile_type::grassland, tile_type::grassland) == tile_type::hills);
    //assert(get_merge_type(tile_type::hills, tile_type::hills) == tile_type::mountains);
    assert(get_merge_type(tile_type::grassland, tile_type::desert) == tile_type::savannah);
    assert(get_merge_type(tile_type::desert, tile_type::grassland) == tile_type::savannah);
    assert(get_merge_type(tile_type::grassland, tile_type::water) == tile_type::swamp);
    assert(get_merge_type(tile_type::water, tile_type::grassland) == tile_type::swamp);
    assert(get_merge_type(tile_type::water, tile_type::mountains) == tile_type::arctic);
    assert(get_merge_type(tile_type::mountains, tile_type::water) == tile_type::arctic);
    //assert(get_merge_type(tile_type::water, tile_type::woods) == tile_type::mangrove);
    //assert(get_merge_type(tile_type::woods, tile_type::water) == tile_type::mangrove);
    //assert(get_merge_type(tile_type::hills, tile_type::beach) == tile_type::dunes);
    //assert(get_merge_type(tile_type::beach, tile_type::hills) == tile_type::dunes);
    //assert(get_merge_type(tile_type::dessert, tile_type::water) == tile_type::beach);
    //assert(get_merge_type(tile_type::water, tile_type::dessert) == tile_type::beach);
    //assert(get_merge_type(tile_type::mangrove, tile_type::grasslands) == tile_type::swamp);
    //assert(get_merge_type(tile_type::grasslands, tile_type::mangrove) == tile_type::swamp);
    //assert(get_merge_type(tile_type::water, tile_type::dunes) == tile_type::beach);
    //assert(get_merge_type(tile_type::dunes, tile_type::water) == tile_type::beach);
    //assert(get_merge_type(tile_type::woods, tile_type::woods) == tile_type::rain_forrest);
    //assert(get_merge_type(tile_type::savannah, tile_type::arctic) == tile_type::tundra);
    //assert(get_merge_type(tile_type::arctic, tile_type::savannah) == tile_type::tundra);
    //TODO: after Issue #187: test more combinations
  }
  // Convert all tile types to string and back
  {
    const std::vector<tile_type> v = get_all_tile_types();
    for (const tile_type t : v)
    {
      const std::string s = to_str(t);
      const tile_type u = to_tile(s);
      assert(t == u);
    }
  }
}

std::vector<tile_type> get_all_tile_types() noexcept
{
  return{
  tile_type::nonetile,
  tile_type::grassland,
  tile_type::mountains,
  tile_type::water,
  tile_type::savannah,
  tile_type::arctic,
  tile_type::desert,
  tile_type::swamp,
  tile_type::dunes,
  tile_type::mangrove,
  tile_type::beach,
  tile_type::rainforest,
  tile_type::hills,
  tile_type::tundra,
  };
}

std::string to_str(tile_type t) //!OCLINT cannot be simpler
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
    case tile_type::water:
      return "water";
    case tile_type::savannah:
      return "savannah";
    case tile_type::swamp:
      return "swamp";
    case tile_type::woods:
      return "woods";
  case tile_type::dunes:
    return "dunes";
  case tile_type::mangrove:
    return "mangrove";
    case tile_type::beach:
      return "beach";
    case tile_type::hills:
      return "hills";
    case tile_type::rainforest:
      return "rainforest";
    case tile_type::tundra:
      return "tundra";
    default:
      assert(t == tile_type::nonetile);
      return "nonetile";
  }
}

tile_type to_tile(std::string str) //!OCLINT NPath Complexity Number 256 exceeds limit of 200
{
  if (str == "arctic") return tile_type::arctic;
  if (str == "grassland") return tile_type::grassland;
  if (str == "desert") return tile_type::desert;
  if (str == "swamp") return tile_type::swamp;
  if (str == "mountains") return tile_type::mountains;
  if (str == "water") return tile_type::water;
  if (str == "savannah") return tile_type::savannah;
  if (str == "woods") return tile_type::woods;
  if (str == "dunes") return tile_type::dunes;
  if (str == "mangrove") return tile_type::mangrove;
  if (str == "beach") return tile_type::beach;
  if (str == "hills") return tile_type::hills;
  if (str == "tundra") return tile_type::tundra;
  if (str == "rainforest") return tile_type::rainforest;
  return tile_type::nonetile;
}

std::ostream& operator <<(std::ostream& os, const tile_type t) noexcept
{
  os << to_str(t);
  return os;
}

std::istream& operator>>(std::istream& is, tile_type& t) noexcept
{
  std::string str;
  is >> str;
  t = to_tile(str); //!OCLINT indeed parameter reassignment, which is exactly the idea
  return is;
}
