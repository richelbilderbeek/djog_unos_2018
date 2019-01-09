#ifndef AGENT_H
#define AGENT_H

#include <vector>

#include "SFML/Graphics.hpp"
#include "agent_type.h"

//Forward declaration
class game;

/// Logic of an agent; something that moves on the tiles
class agent {
public:
  /// Constructor
  /// @param x the x-coordinat of the top-left corner of the agent
  /// @param y the y-coordinat of the top-left corner of the agent
  /// @param type the type the tile
  agent(const agent_type type, const double x = 0.0, const double y = 0.0, double health = 1.0);

  /// The type the tile
  agent_type get_type() const noexcept { return m_type; }

  double get_health() const noexcept { return m_health; }

  double get_stamina() const noexcept { return m_stamina; }

  /// The x-coordinat of the top-left corner of the agent
  double get_x() const noexcept { return m_x; }

  /// The y-coordinat of the top-left corner of the agent
  double get_y() const noexcept { return m_y; }

  void set_x(double x) noexcept { m_x = x; }
  void set_y(double y) noexcept { m_y = y; }
  void set_health(double health) noexcept {m_health = health; }

  /// Check if the agent wants to move to position
  bool checkout(double x, double y);

  bool can_eat(agent &a);
  bool run_away(agent &a);

  /// Moves an agent. It can read the game, containing
  /// agents and tiles for its movement
  void move(/*const game& g*/);

  bool is_clicked(const double x, const double y, const sf::Texture& sprite) const noexcept;

private:
  /// The type the tile
  agent_type m_type;

  /// The x-coordinat of the top-left corner of the agent
  double m_x;

  /// The y-coordinat of the top-left corner of the agent
  double m_y;

  /// The health of the agent
  double m_health;
  ///the stamina of the agent
  double m_stamina;

  friend std::ostream& operator<<(std::ostream& os, const agent& a) noexcept;
  friend std::istream& operator>>(std::istream& is, agent& a);
  friend bool operator==(const agent& lhs, const agent& rhs) noexcept;

};

std::vector<agent> create_default_agents() noexcept;

void move_agent_to_tile(agent &a, double tile_x, double tile_y);

/// Test the tile class
void test_agent();

bool operator==(const agent& lhs, const agent& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const agent& a) noexcept;
std::istream& operator>>(std::istream& is, agent& a);

#endif // TILE_H
