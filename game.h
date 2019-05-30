#ifndef GAME_H
#define GAME_H

#include <vector>

#include "tile.h"
#include "agent.h"
#include "sound_type.h"
#include "sfml_camera.h"
#include <QDir>

class game { //!OCLINT too many methods

friend class sfml_game;

public:
  /// Constructor
  game(
    const std::vector<tile>& tiles = create_test_default_tiles(),
    const std::vector<agent>& agents = create_default_agents()
  );

  ///Does the game spawn agents?
  ///Iff true, tiles spawn agents.
  ///Spawning is set to false in debugging,
  ///e.g. when all creatures must go extinct due to starvation
  bool allow_spawning() const noexcept { return m_allow_spawning; }

  bool allow_damage() const noexcept { return m_allow_damage; }

  bool allow_score() const noexcept { return m_allow_score; }

  /// Read all tiles
  const auto &get_tiles() const noexcept { return m_tiles; }
  auto &get_tiles() noexcept { return m_tiles; }

  /// Read all agents
  const auto &get_agents() const noexcept { return m_agents; }
  auto &get_agents() noexcept { return m_agents; }

  double get_score() const noexcept { return m_score; }

  // No defenition???
  //void change_score_by(int delta_score);

  int get_essence() const noexcept { return m_essence; }

  void delete_tiles(std::vector<tile> ts);

  void add_agents(const std::vector<agent>& as);

  int get_n_ticks() const;

  /// Check whether the clicked tile can be selected and if so select it
  void check_selection(double mouse_X, double mouse_y);

  void confirm_tile_move(tile& t, int direction, int tile_speed);

  /// Timer, physics, bullets moving, etc.bool
  /// Everything except user input.
  void process_events(sound_type &st);

  /// Set m_sound_type to the input sound_type
  void set_sound_type(const sound_type st) noexcept
  { m_sound_type = st; }

  /// See what the sound_type of m_sound_type is
  sound_type get_sound_type() noexcept
  { return m_sound_type; }

  void remove_tile(double mouse_x, double mouse_y);

  int get_agent_count(agent_type type);

  bool is_selected();

  // No defenition???
  //void spawn(agent_type type, tile t);

  /// Allow the real game to allow spawning of agents
  void set_allow_spawning(const bool do_allow) noexcept { m_allow_spawning = do_allow; }

  void set_allow_damage(const bool do_damage) noexcept { m_allow_damage = do_damage; }

  void set_allow_score(const bool do_score) noexcept { m_allow_score = do_score; }

  void save_this(const std::string filename) const;  

private:

  ///Does the game spawn agents?
  ///Iff true, tiles spawn agents.
  ///Spawning is set to false in debugging,
  ///e.g. when all creatures must go extinct due to starvation
  bool m_allow_spawning = true;

  ///Allow agents to damage each other
  bool m_allow_damage = true;

  ///Update the score
  bool m_allow_score = true;

  /// The selected tile
  std::vector<int> m_selected;

  void tile_merge(tile& focal_tile, const tile& other_tile, const int other_pos);

  void merge_tiles(sound_type& st);

  void kill_agents();

  /// Tiles list
  std::vector<tile> m_tiles;

  /// Agents list
  std::vector<agent> m_agents;

  int m_n_tick = 0;

  double m_score;

  int m_essence;

  sound_type m_sound_type;

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

int random_int(int min, int max, unsigned seed = 314);
double random_double(double min, double max, unsigned seed = 314);

/// Determine if an agent is on a tile
bool is_on_tile(const game& g, const agent& a);

/// Determine if there is a tile at the given coordinat
bool is_on_tile(const game& g, double x, double y);

/// Get the tile_type the agent is one.
/// Returns one tile_type if the agent is on a tile.
/// Returns an empty vector if the agent is above the void
std::vector<tile_type> get_on_tile_type(const game& g, const agent& a);
std::vector<tile_type> get_on_tile_type(const game& g, const double x, const double y);

sf::Vector2f get_agent_center(const agent& a);

double get_agent_width(const sf::Texture& a);

double get_agent_height(const sf::Texture& a);

/// Determine if an agent is on a specific tile
bool is_on_specific_tile(const agent& a, const tile& t);

/// Determine if there is a specific tile at the given coordinat
bool is_on_specific_tile(double x, double y, const tile& t);

/// Gets a tile at the coordinats of the agent.
/// If there is at least one tile at the specified coordinats,
/// a vector with a copy of the first tile found is returned.
/// If there are not tiles at the specified coordinats,
/// an empty vector is returned
std::vector<tile> get_current_tile(game& g, const agent& a);

/// Gets a tile at the specified coordinats.
/// If there is at least one tile at the specified coordinats,
/// a vector with a copy of the first tile found is returned.
/// If there are not tiles at the specified coordinats,
/// an empty vector is returned
std::vector<tile> get_current_tile(game& g, double x, double y);

/// Load a game from a file
game load(const std::string &filename);

std::vector<std::string> get_saves();

const std::string SAVE_DIR = "saves\\";

/// Save the game to a file
void save(const game &game, const std::string &filename);

std::vector<std::string> get_saves();

/// Test the game class
void test_game();

sf::Time deltatime();

std::ostream& operator<<(std::ostream& os, const game& g);

std::istream& operator>>(std::istream& os, game& g);

bool operator==(const game& lhs, const game& rhs) noexcept;
bool operator!=(const game& lhs, const game& rhs) noexcept;

#endif // GAME_H
