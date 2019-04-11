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
#include <tuple>
#include <map>

std::vector<tile_type> get_merge_type(tile_type type1, tile_type type2) noexcept //!OCLINT must be simpler
{
  using triplet = std::tuple<tile_type, tile_type, tile_type>;
  const std::vector<triplet> v = {
    //Ordered alphabetically
    triplet(tile_type::arctic, tile_type::grassland, tile_type::tundra),
    triplet(tile_type::arctic, tile_type::mountains, tile_type::tundra),
    triplet(tile_type::arctic, tile_type::savannah, tile_type::tundra),
    triplet(tile_type::arctic, tile_type::swamp, tile_type::tundra),
    triplet(tile_type::arctic, tile_type::woods, tile_type::tundra),
    triplet(tile_type::beach, tile_type::hills, tile_type::dunes),
    triplet(tile_type::desert, tile_type::grassland, tile_type::savannah),
    triplet(tile_type::desert, tile_type::water, tile_type::beach),
    triplet(tile_type::dunes, tile_type::water, tile_type::beach),
    triplet(tile_type::grassland, tile_type::arctic, tile_type::tundra),
    triplet(tile_type::grassland, tile_type::desert, tile_type::savannah),
    triplet(tile_type::grassland, tile_type::grassland, tile_type::hills),
    triplet(tile_type::grassland, tile_type::mangrove, tile_type::swamp),
    triplet(tile_type::grassland, tile_type::water, tile_type::swamp),
    triplet(tile_type::hills, tile_type::beach, tile_type::dunes),
    triplet(tile_type::hills, tile_type::hills, tile_type::mountains),
    triplet(tile_type::mangrove, tile_type::grassland, tile_type::swamp),
    triplet(tile_type::mountains, tile_type::arctic, tile_type::tundra),
    triplet(tile_type::mountains, tile_type::water, tile_type::arctic),
    triplet(tile_type::rainforest, tile_type::water, tile_type::mangrove),
    triplet(tile_type::savannah, tile_type::arctic, tile_type::tundra),
    triplet(tile_type::swamp, tile_type::arctic, tile_type::tundra),
    triplet(tile_type::water, tile_type::desert, tile_type::beach),
    triplet(tile_type::water, tile_type::dunes, tile_type::beach),
    triplet(tile_type::water, tile_type::grassland, tile_type::swamp),
    triplet(tile_type::water, tile_type::rainforest, tile_type::mangrove),
    triplet(tile_type::water, tile_type::mountains, tile_type::arctic),
    triplet(tile_type::water, tile_type::woods, tile_type::mangrove),
    triplet(tile_type::woods, tile_type::arctic, tile_type::tundra),
    triplet(tile_type::woods, tile_type::water, tile_type::mangrove),
    triplet(tile_type::woods, tile_type::woods, tile_type::rainforest),
  };
  for (const auto& t: v)
  {
    if (std::get<0>(t) == type1 && std::get<1>(t) == type2)
    {
      return { std::get<2>(t) };
    }
    if (std::get<0>(t) == type2 && std::get<1>(t) == type1)
    {
      return {std::get<2>(t)};
    }
  }
  return {};
}

void test_tile_type() //!OCLINT testing functions can be long and complex
{
  {
    // merging of types
    assert(get_merge_type(tile_type::grassland, tile_type::grassland).front() == tile_type::hills);
    assert(get_merge_type(tile_type::hills, tile_type::hills).front() == tile_type::mountains);
    assert(get_merge_type(tile_type::grassland, tile_type::desert).front() == tile_type::savannah);
    assert(get_merge_type(tile_type::desert, tile_type::grassland).front() == tile_type::savannah);
    assert(get_merge_type(tile_type::grassland, tile_type::water).front() == tile_type::swamp);
    assert(get_merge_type(tile_type::water, tile_type::grassland).front() == tile_type::swamp);
    assert(get_merge_type(tile_type::water, tile_type::mountains).front() == tile_type::arctic);
    assert(get_merge_type(tile_type::mountains, tile_type::water).front() == tile_type::arctic);
    assert(get_merge_type(tile_type::water, tile_type::woods).front() == tile_type::mangrove);
    assert(get_merge_type(tile_type::woods, tile_type::water).front() == tile_type::mangrove);
    assert(get_merge_type(tile_type::hills, tile_type::beach).front() == tile_type::dunes);
    assert(get_merge_type(tile_type::beach, tile_type::hills).front() == tile_type::dunes);
    assert(get_merge_type(tile_type::desert, tile_type::water).front() == tile_type::beach);
    assert(get_merge_type(tile_type::water, tile_type::desert).front() == tile_type::beach);
    assert(get_merge_type(tile_type::mangrove, tile_type::grassland).front() == tile_type::swamp);
    assert(get_merge_type(tile_type::grassland, tile_type::mangrove).front() == tile_type::swamp);
    assert(get_merge_type(tile_type::water, tile_type::dunes).front() == tile_type::beach);
    assert(get_merge_type(tile_type::dunes, tile_type::water).front() == tile_type::beach);
    assert(get_merge_type(tile_type::woods, tile_type::woods).front() == tile_type::rainforest);
    assert(get_merge_type(tile_type::savannah, tile_type::arctic).front() == tile_type::tundra);
    assert(get_merge_type(tile_type::arctic, tile_type::savannah).front() == tile_type::tundra);
    //If you miss a combination, this is the place to add a test :-)
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
  return
  {
    tile_type::arctic,
    tile_type::beach,
    tile_type::desert,
    tile_type::dunes,
    tile_type::grassland,
    tile_type::hills,
    tile_type::mangrove,
    tile_type::mountains,
    tile_type::rainforest,
    tile_type::savannah,
    tile_type::swamp,
    tile_type::tundra,
    tile_type::water,
    tile_type::woods
  };
}

std::string to_str(tile_type t) //!OCLINT cannot be simpler
{
  switch (t)
  {
    case tile_type::arctic: return "arctic";
    case tile_type::beach: return "beach";
    case tile_type::desert: return "desert";
    case tile_type::dunes: return "dunes";
    case tile_type::grassland: return "grassland";
    case tile_type::hills: return "hills";
    case tile_type::mangrove: return "mangrove";
    case tile_type::mountains: return "mountains";
    case tile_type::rainforest: return "rainforest";
    case tile_type::savannah: return "savannah";
    case tile_type::swamp: return "swamp";
    case tile_type::tundra: return "tundra";
    case tile_type::water: return "water";
    case tile_type::woods: return "woods";
  }
  assert(!"Should not get here"); //!OCLINT acceptable idiom
  return "";
}

tile_type to_tile(std::string str) //!OCLINT NPath Complexity Number 256 exceeds limit of 200
{
  const std::map<std::string, tile_type> m{
    { "arctic", tile_type::arctic},
    { "desert", tile_type::desert},
    { "dunes", tile_type::dunes},
    { "grassland", tile_type::grassland},
    { "hills", tile_type::hills},
    { "mangrove", tile_type::mangrove},
    { "mountains", tile_type::mountains},
    { "rainforest", tile_type::rainforest},
    { "savannah", tile_type::savannah},
    { "swamp", tile_type::swamp},
    { "tundra", tile_type::tundra},
    { "water", tile_type::water},
    { "woods", tile_type::woods},
    { "beach", tile_type::beach} //Other tile types here
  };
  //This assert will fail if the string is not in the map
  std::clog << str << std::endl;
  assert(m.find(str) != std::end(m));
  return m.find(str)->second;
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
