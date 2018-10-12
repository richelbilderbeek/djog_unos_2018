#ifndef TILE_H
#define TILE_H

#include "agent.h"
#include "tile_type.h"
#include <vector>

/// A world tile
class tile {
public:
  /// Constructor
  /// @param x the x-coordinate of the top-left corner of the tile
  /// @param y the y-coordinate of the top-left corner of the tile
  /// @param width the width of the tile
  /// @param height the height of the tile
  /// @param type the type the tile
  /// @param the tiles id
  tile(const double x, const double y, const double width, const double height,
       const tile_type type, const int id);

  /// Read all agents
  const std::vector<agent> &get_agents() const noexcept { return m_agents; }

  /// The height of the tile
  double get_height() const noexcept { return m_height; }

  /// The type the tile
  tile_type get_type() const noexcept { return m_type; }

  /// The width of the tile
  double get_width() const noexcept { return m_width; }

  /// The x-coordinat of the top-left corner of the tile
  double get_x() const noexcept { return m_x; }

  /// The y-coordinat of the top-left corner of the tile
  double get_y() const noexcept { return m_y; }

  /// The movement coeficient on the x-axis
  double get_dx() const noexcept { return m_dx; }

  /// The movement coeficient on the y-axis
  double get_dy() const noexcept { return m_dy; }

  /// Set the movement coeficient on the x-axis
  void set_dx(double dx);

  /// Set the movement coeficient on the y-axis
  void set_dy(double dy);

  /// Move the tile by the movement coeficients
  void move();

  void add_agent(agent a);

  /// Get the tile's id
  int get_id() const noexcept { return m_id; }

  void set_id(int tid);

  /// Is this coordinate within the tile?
  bool tile_contains(double x, double y) const noexcept;

  void lock_movement(bool b);

  bool get_m_locked() const noexcept { return m_locked; }

private:
  /// The height of the tile
  const double m_height;

  /// The type the tile
  const tile_type m_type;

  /// The width of the tile
  const double m_width;

  /// The x-coordinat of the top-left corner of the tile
  double m_x;

  /// The y-coordinat of the top-left corner of the tile
  double m_y;

  /// The movement coeficient on the x-axis
  double m_dx;

  /// The movement coeficient on the y-axis
  double m_dy;

  /// Agents list
  std::vector<agent> m_agents;

  /// The tile's id
  int m_id;

  bool m_locked = false;
};

/// Test the tile class
void test_tile();

#endif // TILE_H
