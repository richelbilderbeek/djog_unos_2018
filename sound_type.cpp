#include "sound_type.h"

#include <string>
#include <algorithm>
#include <cassert>

std::vector<sound_type> get_all_sound_types()
{
  return
  {
    sound_type::tile_move,
    sound_type::tile_rotate,
    sound_type::tile_collision
  };
}

std::string to_string(const sound_type& st)
{
  switch (st)
  {
    case sound_type::tile_move:
      return "tile_move";
    case sound_type::tile_rotate:
      return "tile_rotate";
    case sound_type::tile_collision:
      return "tile_collision";
    default:
      return "no_sound";
  }
}

void test_sound_type()
{
  const std::vector<sound_type> st
  { get_all_sound_types() };

   assert(std::count(std::begin(st), std::end(st), sound_type::tile_move) == 1);
   assert(std::count(std::begin(st), std::end(st), sound_type::tile_rotate) == 1);
   assert(std::count(std::begin(st), std::end(st), sound_type::tile_collision) == 1);
}
