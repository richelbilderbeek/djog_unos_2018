#ifndef TILE_H
#define TILE_H

#include "agent.h"
#include "tile_id.h"
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
  /// @param depth the depth of the tile
  /// @param type the type the tile
  /// @param the tiles id
  tile(double x = 0.0, double y = 0.0, const double z = 0.0,
       double rotation = 0.0, double depth = 0.0,
       const tile_type type = tile_type::grassland, const tile_id = tile_id());

  /// The height of the tile
  double get_rotation() const noexcept { return m_rotation; }

  /// The type the tile
  tile_type get_type() const noexcept { return m_type; }

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

  /// The tile's depth
  double get_depth() const noexcept { return m_depth; }

  /// The center of the tile
  sf::Vector2f get_center() const noexcept;

  ///Process events, for example, make the agents move
  void process_events(game& g);

  void spawn(game& g, agent_type type);

  /// Set the movement coeficient on the x-axis
  void set_dx(double dx);

  /// Set the movement coeficient on the y-axis
  void set_dy(double dy);

  void set_rotation(double r);//TODO

  void rotate_c();//TODO
  void rotate_cc();//TODO

  double get_width() const;
  double get_height() const;

  /// Set the movement coeficient of the z-axis
  void set_dz(double dz);

  /// Set the type of the tile
  void set_type(const tile_type t) noexcept;

  /// Move the tile by the movement coeficients
  void move(std::vector<agent>& a);

  void move();

  /// Get the tile's id
  int get_id() const noexcept { return m_id.get(); }


  void lock_movement(bool b);

  bool get_m_locked() const noexcept { return m_locked; }

private:

  /// The tile's depth
  double m_depth;

  /// The movement coefficient on the x-axis
  double m_dx{0.0};

  /// The movement coefficient on the y-axis
  double m_dy{0.0};

  /// The movement coefficient on the z-axis
  double m_dz{0.0};

  /// The tile's id
  tile_id m_id;

  bool m_locked{false};

  /// The type the tile
  tile_type m_type;

  /// The width of the tile
  double m_rotation;

  /// The x-coordinate of the top-left corner of the tile
  double m_x;

  /// The y-coordinate of the top-left corner of the tile
  double m_y;

  /// The z-coordinate of the top-left corner of the tile
  double m_z;

  int ticks = 1;

  //A rare exception to use a friend
  friend std::ostream& operator<<(std::ostream& os, const tile& t);
  friend std::istream& operator>>(std::istream& os, tile& t);
  friend bool operator==(const tile& lhs, const tile& rhs) noexcept;

};

/// Create the default collection of tiles
std::vector<tile> create_default_tiles() noexcept;

/// Create the default collection of tiles (old)
std::vector<tile> create_test_default_tiles() noexcept;

/// Create a tile to be used in testing:
///
///   (0,0)-------(1,0)
///     |           |
///     | grassland |
///     |           |
///   (0,1)-------(1,1)
///
/// * z: 0.0
/// * depth: 0.0
tile create_test_tile() noexcept;

/// Create two horizontally adjacent grass tiles
/// +----+----+----+----+
/// |  grass  |  grass  |
/// +----+----+----+----+
std::vector<tile> create_two_grass_tiles() noexcept;

/// Do these tiles have the same position?
bool have_same_position(const tile& lhs, const tile& rhs) noexcept;

/// Is this coordinate within the tile?
bool contains(const tile& t, double x, double y) noexcept;

std::ostream& operator<<(std::ostream& os, const tile& t);

std::istream& operator>>(std::istream& os, tile& t);

bool operator==(const tile& lhs, const tile& rhs) noexcept;

/// Test the tile class
void test_tile();

#endif // TILE_H
