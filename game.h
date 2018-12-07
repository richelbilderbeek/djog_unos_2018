#ifndef GAME_H
#define GAME_H

#include <vector>

#include "tile.h"
#include "agent.h"

class game {

public:
  /// Constructor
  game(const std::vector<tile>& tiles = create_default_tiles(),
       const std::vector<agent>& agents = create_default_agents());

  /// Read all tiles
  const auto &get_tiles() const noexcept { return m_tiles; }
  auto &get_tiles() noexcept { return m_tiles; }

  /// Read all agents
  const auto &get_agents() const noexcept { return m_agents; }
  auto &get_agents() noexcept { return m_agents; }

  int get_score() const noexcept { return m_score; }

  void change_score_by(int delta_score);

  void add_tiles(std::vector<tile> ts);
  void delete_tiles(std::vector<tile> ts);

  void add_agents(std::vector<agent> as);



  /// Timer, physics, bullets moving, etc.
  /// Everything except user input.
  void process_events();

  /// The selected tile
  std::vector<int> m_selected;

private:

  void merge_tiles();
  /// Tiles list
  std::vector<tile> m_tiles;

  /// Agents list
  std::vector<agent> m_agents;

  int m_n_tick = 0;

  int m_score;

  //A rare exception to use a friend
  friend std::ostream& operator<<(std::ostream& os, const game& g);
  friend std::istream& operator>>(std::istream& os, game& g);

  friend bool operator==(const game& lhs, const game& rhs) noexcept;
};

/// Collect all the tiles' types in the game
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
