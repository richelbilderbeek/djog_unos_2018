#ifndef GAME_H
#define GAME_H

#include <vector>

#include "tile.h"

class game
{

public:
  ///Constructor
  game();

  ///Read all tiles
  const auto& get_tiles() const noexcept { return m_tiles; }

  int get_score() const noexcept { return m_score; }

  void change_score_by(int delta_score);

  int new_id() {
    ++old_id;
    return old_id;
  }

private:

  ///Timer, physics, bullets moving, etc.
  ///Everything except user input.
  void process_events();

  std::vector<tile> m_tiles;

  int m_n_tick = 0;

  int m_score;

  int old_id = 0;
};

///Test the game class
void test_game();

#endif // GAME_H
