#ifndef GAME_H
#define GAME_H

#include <vector>

#include "tile.h"

class game {

public:
  /// Constructor
  game();

  /// Read all tiles
  const auto &get_tiles() const noexcept { return m_tiles; }
  auto &get_tiles() noexcept { return m_tiles; }

  int get_score() const noexcept { return m_score; }

  void change_score_by(int delta_score);

  int new_id() {
    ++old_id;
    return old_id;
  }

  void add_tiles(std::vector<tile> ts);

  int old_id = 0;

  /// Timer, physics, bullets moving, etc.
  /// Everything except user input.
  void process_events();

private:

  std::vector<tile> m_tiles;

  int m_n_tick = 0;

  int m_score;


  //A rare exception to use a friend
  friend std::ostream& operator<<(std::ostream& os, const game& g);
  friend std::istream& operator>>(std::istream& os, game& g);
};

/// Load a game from a file
game load(const std::string &filename);

/// Save the game to a file
void save(const game &game, const std::string &filename);

/// Test the game class
void test_game();

std::ostream& operator<<(std::ostream& os, const game& g);

std::istream& operator>>(std::istream& os, game& g);

#endif // GAME_H
