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
  {
    //Uncomment if you want to run this test
    //#define FIX_ISSUE_188 // TODO complete the function so this works again
    #ifdef FIX_ISSUE_188
    const std::vector<tile_type> v = get_all_tile_types();
    for (const tile_type t : v)
    {
      const std::string s = to_str(t);
      const tile_type u = to_tile(s);
      assert(t == u);
    }
    #endif // FIX_ISSUE_188
  }
}

std::vector<tile_type> get_all_tile_types() noexcept
{
  std::vector<tile_type> v;
  v.push_back(tile_type::nonetile);
  v.push_back(tile_type::grassland);
  v.push_back(tile_type::mountains);
  v.push_back(tile_type::water);
  v.push_back(tile_type::savannah);
  v.push_back(tile_type::arctic);
  v.push_back(tile_type::desert);
  v.push_back(tile_type::swamp);
  v.push_back(tile_type::Dunes);
  v.push_back(tile_type::mangrove);
  v.push_back(tile_type::beach);
  v.push_back(tile_type::rainforest);
  v.push_back(tile_type::hills);
  v.push_back(tile_type::tundra);
  return v;
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
  case tile_type::Dunes:
    return "Dunes";
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
  if (str == "Dunes") return tile_type::Dunes;
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
