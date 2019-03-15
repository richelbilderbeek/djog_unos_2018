#ifndef GAME_H
#define GAME_H

#include <vector>

#include "tile.h"
#include "agent.h"
#include "sfml_camera.h"
#include <QDir>

class game {

friend class sfml_game;

public:
  /// Constructor
  game(
    const std::vector<tile>& tiles = create_default_tiles(),
    const std::vector<agent>& agents = create_default_agents()
  );

  ///Does the game spawn agents?
  ///Iff true, tiles spawn agents.
  ///Spawning is set to false in debugging,
  ///e.g. when all creatures must go extinct due to starvation
  bool allow_spawning() const noexcept { return m_allow_spawning; }

  /// Read all tiles
  const auto &get_tiles() const noexcept { return m_tiles; }
  auto &get_tiles() noexcept { return m_tiles; }

  /// Read all agents
  const auto &get_agents() const noexcept { return m_agents; }
  auto &get_agents() noexcept { return m_agents; }

  double get_score() const noexcept { return m_score; }

  void change_score_by(int delta_score);

  int get_essence() const noexcept { return m_essence; }

  void delete_tiles(std::vector<tile> ts);

  void add_agents(const std::vector<agent>& as);

  int get_n_ticks() const;

  void move_tiles(sf::RenderWindow& window, sfml_camera& camera);

  void confirm_tile_move(tile& t, int direction, int tile_speed);

  /// Timer, physics, bullets moving, etc.
  /// Everything except user input.
  void process_events();

  void remove_tile(sf::RenderWindow& window, sfml_camera& camera);

  int get_agent_count(agent_type type);

  void spawn(agent_type type, tile t);

  /// Allow the real game to allow spawning of agents
  void set_allow_spawning(const bool do_allow) noexcept { m_allow_spawning = do_allow; }

private:

  ///Does the game spawn agents?
  ///Iff true, tiles spawn agents.
  ///Spawning is set to false in debugging,
  ///e.g. when all creatures must go extinct due to starvation
  bool m_allow_spawning;

  /// The selected tile
  std::vector<int> m_selected;

  void tile_merge(tile& focal_tile, const tile& other_tile, const int other_pos);

  void merge_tiles();

  void kill_agents();

  /// Tiles list
  std::vector<tile> m_tiles;

  /// Agents list
  std::vector<agent> m_agents;

  int m_n_tick = 0;

  double m_score;

  int m_essence;

  //A rare exception to use a friend
  friend std::ostream& operator<<(std::ostream& os, const game& g);
  friend std::istream& operator>>(std::istream& os, game& g);

  friend bool operator==(const game& lhs, const game& rhs) noexcept;
};

/// Collect all the tiles' types in the game
std::vector<tile_type> collect_tile_types(const game& g) noexcept;

/// Count the number of tiles a game has
int count_n_tiles(const game& g) noexcept;

int count_n_agents(const game& g) noexcept;

/// Determine if an agent is on a tile
bool is_on_tile(const game& g, const agent& a);

/// Determine if there is a tile at the given coordinat
bool is_on_tile(const game& g, double x, double y);

tile_type get_on_tile_type(const game& g, const agent& a);

/// Determine if an agent is on a specific tile
bool is_on_specific_tile(const agent& a, const tile& t);

/// Determine if there is a specific tile at the given coordinat
bool is_on_specific_tile(double x, double y, const tile& t);

tile get_current_tile(game& g, const agent& a);

tile get_current_tile(game& g, double x, double y);

/// Load a game from a file
game load(const std::string &filename);

// The save Subdir
const std::string SAVE_DIR = "saves\\";

/// Save the game to a file
void save(const game &game, const std::string &filename);

/// Test the game class
void test_game();

sf::Time deltatime();

std::ostream& operator<<(std::ostream& os, const game& g);

std::istream& operator>>(std::istream& os, game& g);

bool operator==(const game& lhs, const game& rhs) noexcept;

#endif // GAME_H
