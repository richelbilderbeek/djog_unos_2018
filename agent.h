#ifndef AGENT_H
#define AGENT_H

#include <vector>

#include "agent_type.h"

///A world tile
class agent
{
public:
  ///Constructor
  /// @param x the x-coordinat of the top-left corner of the agent
  /// @param y the y-coordinat of the top-left corner of the agent
  /// @param type the type the tile
  agent(
    const agent_type type,
    const double x = 0,
    const double y = 0
  );

  /// The type the tile
  agent_type get_type() const noexcept { return m_type; }

  /// The x-coordinat of the top-left corner of the agent
  double get_x() const noexcept { return m_x; }

  /// The y-coordinat of the top-left corner of the agent
  double get_y() const noexcept { return m_y; }

  // TODO Add below functions to the cpp file (#33) -Joshua

  /// Check if the agent wants to move to position
  bool checkout(double x, double y);

  /// Make the agent move (movement depends on agent type and surroundings)
  void action();

  bool can_eat(agent& a);
  bool run_away(agent& a);

private:

  /// The type the tile
  const agent_type m_type;

  /// The x-coordinat of the top-left corner of the agent
  double m_x;

  /// The y-coordinat of the top-left corner of the agent
  double m_y;

};

///Test the tile class
void test_agent();

#endif // TILE_H
