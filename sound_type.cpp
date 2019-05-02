#include "sound_type.h"

#include <string>
#include <algorithm>
#include <cassert>

std::vector<sound_type> get_all_sound_types()
{
  return
  {
    sound_type::none,
    sound_type::tile_move,
    sound_type::tile_rotate,
    sound_type::tile_collision,
    sound_type::random_animal
  };
}

void test_sound_type()
{
  const std::vector<sound_type> sts
  { get_all_sound_types() };

  assert(std::count(std::begin(sts), std::end(sts), sound_type::none) == 1);
  assert(std::count(std::begin(sts), std::end(sts), sound_type::tile_move) == 1);
  assert(std::count(std::begin(sts), std::end(sts), sound_type::tile_rotate) == 1);
  assert(std::count(std::begin(sts), std::end(sts), sound_type::tile_collision) == 1);
  assert(std::count(std::begin(sts), std::end(sts), sound_type::random_animal) == 1);
}
