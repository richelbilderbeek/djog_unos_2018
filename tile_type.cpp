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
    return tile_type::mountains;
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
  return tile_type::nonetile;
}

void test_tile_type()
{
  {
    // merging of types
    assert(get_merge_type(tile_type::nonetile, tile_type::grassland) == tile_type::nonetile);
    assert(get_merge_type(tile_type::grassland, tile_type::grassland) == tile_type::mountains);
    assert(get_merge_type(tile_type::grassland, tile_type::desert) == tile_type::savannah);
    assert(get_merge_type(tile_type::desert, tile_type::grassland) == tile_type::savannah);
    assert(get_merge_type(tile_type::grassland, tile_type::water) == tile_type::swamp);
    assert(get_merge_type(tile_type::water, tile_type::grassland) == tile_type::swamp);
    //TODO: after Issue #187: test more combinations
  }
  {
    //Uncomment if you want to run this test
    #define FIX_ISSUE_188
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
