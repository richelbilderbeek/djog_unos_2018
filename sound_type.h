#ifndef SOUND_TYPE_H
#define SOUND_TYPE_H

#include <iosfwd>
#include <vector>

enum class sound_type
{
  tile_move,
  tile_rotate,
  tile_collision
};

/// Get all sound types
std::vector<sound_type> get_all_sound_types();

/// Test the sound_type class
void test_sound_type();

#endif // SOUND_TYPE_H
