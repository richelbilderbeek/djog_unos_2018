#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>
#include <vector>

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

/// Collect all the game states in the enum class 'game_state'
std::vector<game_state> collect_all_game_states();

/// Test the game_state free functions
void test_game_state();

/// Convert a game_state to a string
game_state to_game_state(std::string);

/// Convert a game_state to a word
std::string to_str(const game_state s) noexcept;

std::ostream& operator<<(std::ostream& os, const game_state s);

#endif // GAME_STATE_H
