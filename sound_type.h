#ifndef SOUND_TYPE_H
#define SOUND_TYPE_H

#include <vector>

enum class sound_type
{
  tile_move,
  tile_rotate,
  tile_collision
};

std::vector<sound_type> get_all_sound_types();

void test_sound_type();

#endif // SOUND_TYPE_H
