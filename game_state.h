#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>

enum class game_state
{
  aboutscreen,
  gameover,
  loading,
  menuscreen,
  paused,
  playing,
  saving,
  shop,
  titlescreen
};

/// Convert a game_state to a word
std::string to_str(const game_state s) noexcept;

std::ostream& operator<<(std::ostream& os, const game_state s);

#endif // GAME_STATE_H
