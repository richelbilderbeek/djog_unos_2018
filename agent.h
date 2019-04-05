#ifndef AGENT_H
#define AGENT_H

#include <vector>

#include "SFML/Graphics.hpp"
#include "agent_type.h"

//Forward declaration
class game;
class tile;

/// Logic of an agent; something that moves on the tiles
class agent {
public:
  /// Constructor
  /// @param x the x-coordinat of the top-left corner of the agent
  /// @param y the y-coordinat of the top-left corner of the agent
  /// @param type the type the tile
  explicit agent(const agent_type type, const double x = 0.0, const double y = 0.0,
        const double health = 1.0,  const double direction = 0.0);

  void process_events(game &g);

  /// The type the tile
  agent_type get_type() const noexcept { return m_type; }

  double get_health() const noexcept { return m_health; }

  double get_stamina() const noexcept { return m_stamina; }

  /// The x-coordinat of the top-left corner of the agent
  double get_x() const noexcept { return m_x; }

  /// The y-coordinat of the top-left corner of the agent
  double get_y() const noexcept { return m_y; }

  /// The direction angle of the agent
  double get_direction() const noexcept { return m_direction; }

  void set_x(double x) noexcept { m_x = x; }
  void set_y(double y) noexcept { m_y = y; }
  void set_direction(double direction) noexcept { m_direction = direction; }
  void set_health(double health) noexcept { m_health = health; }

  sf::Vector2f get_center(const sf::Texture& sprite) const;

  /// Moves an agent. It can read the game, containing
  /// agents and tiles for its movement
  void move();

  void move(double x, double y);

  bool is_clicked(const double x, const double y, const sf::Texture& sprite) const noexcept;

  void eat(const game& g);

  bool is_in_range(double x, double y, double range);

  agent nearest_agent(game& g, agent& a, agent_type type);

  void move_to_food(game& g);

  void attract_to_agent(game& g, agent_type type);

private:
  /// The type the tile
  agent_type m_type;

  /// The x-coordinat of the top-left corner of the agent
  double m_x;

  /// The y-coordinat of the top-left corner of the agent
  double m_y;

  /// The direction angle of the agent:
  ///
  ///           0.5 * pi
  ///              |
  ///              |
  /// 1.0 * pi ----+---- 0.0 * pi
  ///              |
  ///              |
  ///           1.5 * pi
  ///
  /// For example, for an m_direction of 1.5 pi, the agent moves down
  double m_direction;

  /// The health of the agent
  double m_health;
  ///the stamina of the agent
  double m_stamina;

  int corpse_ticks = -1;

  void reproduce_agents(game& g, agent_type type);

  void damage_near_grass(game &g, agent_type type);

  friend std::ostream& operator<<(std::ostream& os, const agent& a) noexcept;
  friend std::istream& operator>>(std::istream& is, agent& a);
  friend bool operator==(const agent& lhs, const agent& rhs) noexcept;

};

std::vector<agent_type> can_eat(const agent_type type);

std::vector<agent> create_default_agents() noexcept;

void move_agent_to_tile(agent &a, double tile_x, double tile_y);

bool will_drown(agent_type a);

/// Determine if the agent_type is a plant
/// @return true if the agent_type is a plant
/// @note plankton is counted as plants, but do include small animals as well
bool is_plant(const agent_type type) noexcept;

int get_min_depth(agent_type a);

int get_max_depth(agent_type a);

sf::Vector2i get_depth(agent_type a);

/// Test the tile class
void test_agent();

bool operator==(const agent& lhs, const agent& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const agent& a) noexcept;
std::istream& operator>>(std::istream& is, agent& a);

#endif // TILE_H
