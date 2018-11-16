#ifndef GAME_H
#define GAME_H

#include <vector>

#include "tile.h"


class game {

public:
  /// Constructor
  game(const std::vector<tile>& tiles = create_default_tiles());

  /// Read all tiles
  const auto &get_tiles() const noexcept { return m_tiles; }
  auto &get_tiles() noexcept { return m_tiles; }

  int get_score() const noexcept { return m_score; }

  void change_score_by(int delta_score);

  void add_tiles(std::vector<tile> ts);
  void delete_tiles(std::vector<tile> ts);

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
  friend bool operator==(const game& lhs, const game& rhs) noexcept;
};

/// Collect all the tiles' types
std::vector<tile_type> collect_tile_types(const game& g) noexcept;

/// Count the number of tiles a game has
int count_n_tiles(const game& g) noexcept;

/// Load a game from a file
game load(const std::string &filename);

/// Save the game to a file
void save(const game &game, const std::string &filename);

/// Test the game class
void test_game();

std::ostream& operator<<(std::ostream& os, const game& g);

std::istream& operator>>(std::istream& os, game& g);

bool operator==(const game& lhs, const game& rhs) noexcept;

#endif // GAME_H
