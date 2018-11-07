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
  /// @param z the z-coordinate of the top-left corner of the tile
  /// @param width the width of the tile
  /// @param height the height of the tile
  /// @param type the type the tile
  /// @param the tiles id
  tile(
       const double x = 0.0, const double y = 0.0, const double z = 0.0,
       const double width = 0.0, const double height = 0.0,
       const tile_type type = tile_type::grassland, const int id = 0);

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

  /// The z-coordinate of the top_left corner of the tile
  double get_z() const noexcept { return m_z; }

  /// The movement coeficient on the x-axis
  double get_dx() const noexcept { return m_dx; }

  /// The movement coeficient on the y-axis
  double get_dy() const noexcept { return m_dy; }

  /// The center of the tile
  Vector2f get_center() const noexcept { return Vector2f(m_width / 2.0f, m_height / 2.0f); }

  ///Process events, for example, make the agents move
  void process_events();

  /// Set the movement coeficient on the x-axis
  void set_dx(double dx);

  /// Set the movement coeficient on the y-axis
  void set_dy(double dy);

  /// Set the movement coeficient of the z-axis
  void set_dz(double dz);

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
  double m_height;

  /// The type the tile
  tile_type m_type;

  /// The width of the tile
  double m_width;

  /// The x-coordinate of the top-left corner of the tile
  double m_x;

  /// The y-coordinate of the top-left corner of the tile
  double m_y;

  /// The z-coordinate of the top-left corner of the tile
  double m_z;

  /// The movement coefficient on the x-axis
  double m_dx;

  /// The movement coefficient on the y-axis
  double m_dy;

  /// The movement coefficient on the z-axis
  double m_dz;

  /// Agents list
  std::vector<agent> m_agents;

  /// The tile's id
  int m_id;

  bool m_locked = false;


  //A rare exception to use a friend
  friend std::ostream& operator<<(std::ostream& os, const tile& t);
  friend std::istream& operator>>(std::istream& os, tile& t);
};

/// Create the default collection of tiles
std::vector<tile> create_default_tiles() noexcept;

std::ostream& operator<<(std::ostream& os, const tile& t);

std::istream& operator>>(std::istream& os, tile& t);

/// Test the tile class
void test_tile();

#endif // TILE_H
