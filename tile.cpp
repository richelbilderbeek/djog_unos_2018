#include "tile.h"
#include "agent.h"
#include "tile_id.h"
#include "tile_type.h"
#include "agent_type.h"
#include "sfml_game.h"
#include "game.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

tile::tile(const double x, const double y, const double z, double const width,
           const double height, const double depth, const tile_type type, const tile_id id)
    : m_height{height}, m_type{type}, m_width{width}, m_x{x}, m_y{y}, m_z{z},
      m_dx{0}, m_dy{0}, m_dz{0}, m_id{id}, m_depth{depth}
{

  if (width <= 0.0)
  {
    throw std::invalid_argument("'width' cannot be negative");
  }

  if (height <= 0.0)
  {
    throw std::invalid_argument("'height' cannot be negative");
  }

  m_x *= 115;
  m_y *= 115;
  m_width = (width * 100) + ((width - 1) * 15);
  m_height = (height * 100) + ((height - 1) * 15);

  assert(m_width > 0.0);
  assert(m_height > 0.0);
}

std::vector<tile> create_default_tiles() noexcept //!OCLINT indeed a function that is too long
{
  std::vector<tile> tiles;
  {
    tile t(0, 0, 0, 1, 2, 0, tile_type::grassland, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(1, 0, 1, 1, 2, 0, tile_type::grassland, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(0, 2, 2, 1, 2, 0, tile_type::desert, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(2, 1, 3, 2, 1, 0, tile_type::swamp, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(1, 2, 4, 2, 1, 0, tile_type::mountains, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(4, 1, 5, 2, 1, 0, tile_type::arctic, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(3, 2, 6, 1, 2, 1, tile_type::water, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(4, 2, 6, 1, 2, 10, tile_type::water, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(1, -1, 7, 2, 1, 0, tile_type::savannah, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(4, -1, 8, 1, 2, 0, tile_type::woods, tile_id());
    tiles.push_back(t);
  }
  return tiles;
}

std::vector<tile> create_two_grass_tiles() noexcept
{
  return
  {
    //   x    y    z   w    h    d   type               ID
    tile(1, 1, 1, 2, 1, 0, tile_type::grassland, tile_id()),
    tile(3, 1, 1, 2, 1, 0, tile_type::grassland, tile_id())
  };
}

bool have_same_position(const tile& lhs, const tile& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && lhs.get_y() == rhs.get_y();
}

void tile::process_events()
{

}

void tile::set_dx(double dx) {
  if (!m_locked)
    m_dx = dx;
}

void tile::set_dy(double dy) {
  if (!m_locked)
    m_dy = dy;
}

void tile::set_type(const tile_type t) noexcept
{
  m_type = t;
}

void tile::move() {
  m_x += m_dx;
  m_y += m_dy;
  m_z += m_dz;
}

std::ostream& operator<<(std::ostream& os, const tile& t)
{
  os << t.m_x << ' ' << t.m_y << ' '
     << t.m_height << ' ' << t.m_width << ' '
     << t.m_locked << ' ' << t.m_type << ' '
     << t.m_dx << ' ' << t.m_dy << ' '
     << t.m_depth;

  return os;
}

std::istream& operator>>(std::istream& is, tile& t)
{
  is >> t.m_x >> t.m_y;
  is >> t.m_height >> t.m_width;
  is >> t.m_locked;
  is >> t.m_type;
  is >> t.m_dx >> t.m_dy;
  is >> t.m_depth;
  return is;
}

bool operator==(const tile& lhs, const tile& rhs) noexcept {
  if (//lhs.m_x != rhs.m_x ||
      //lhs.m_y != rhs.m_y ||
      //lhs.m_z != rhs.m_z ||
      lhs.m_dx != rhs.m_dx ||
      lhs.m_dy != rhs.m_dy ||
      lhs.m_depth != rhs.m_depth ||
      lhs.m_width != rhs.m_width ||
      lhs.m_height != rhs.m_height ||
      lhs.m_locked != rhs.m_locked ||
      lhs.m_type != rhs.m_type)
    return false;
  return true;
}

bool tile::tile_contains(double x, double y) const noexcept {
  return x > m_x - 5 && x < m_x + m_width + 5 && y > m_y - 5 &&
         y < m_y + m_height + 5;
}

void tile::lock_movement(bool b) { m_locked = b; }

void test_tile() //!OCLINT testing function may be many lines
{
  // width cannot be negative
  {
    bool b = false;
    try {
      const tile t(0.0, 0.0, 0.0, -1, 1, 0, tile_type::grassland, tile_id()); //!OCLINT indeed t is unused
    } catch (const std::invalid_argument &e) {
      assert(std::string(e.what()) == "'width' cannot be negative");
      b = true;
    }
    std::cout << b << " "; // use b
    assert(b);
  }
  // height cannot be negative
  {
    bool b = false;
    try {
      const tile t(0.0, 0.0, 0.0, 1, -1, 0, tile_type::grassland, tile_id()); //!OCLINT indeed t is unused
    } catch (const std::invalid_argument &e) {
      assert(std::string(e.what()) == "'height' cannot be negative");
      b = true;
    }
    std::cout << b << "\n"; // use b
    assert(b);
  }
  // A tile starts from standstill
  {
    const tile t(0.0, 0.0, 0.0, 1, 1, 0, tile_type::grassland, tile_id());
    assert(t.get_dx() == 0.0);
    assert(t.get_dy() == 0.0);
  }
  {
    tile t(0.0, 0.0, 0.0, 1, 1, 0, tile_type::grassland, tile_id());
    t.lock_movement(true);
    t.set_dx(12.34);
    t.set_dy(56.78);
    assert(t.get_dx() == 0.0);
    assert(t.get_dy() == 0.0);
  }
  // Speed is set correctly
  {
    tile t(0.0, 0.0, 0.0, 1, 1, 0, tile_type::grassland, tile_id());
    const double dx{12.34};
    const double dy{56.78};
    t.set_dx(dx);
    t.set_dy(dy);
    assert(t.get_dx() == dx);
    assert(t.get_dy() == dy);
  }
  // Tile responds to its speed
  {
    tile t(0.0, 0.0, 0.0, 1, 1, 0, tile_type::grassland, tile_id());
    const double dx{12.34};
    const double dy{56.78};
    t.set_dx(dx);
    t.set_dy(dy);
    assert(t.get_x() != dx);
    assert(t.get_y() != dy);
    t.move();
    assert(t.get_x() == dx);
    assert(t.get_y() == dy);
  }
  // A tile has the correct depth
  {
    tile g(0.0, 0.0, 0.0, 1, 1, 0.0, tile_type::grassland, tile_id());
    assert(g.get_depth() == 0.0);
    tile o(0.0, 0.0, 0.0, 1, 1, 1.0, tile_type::water, tile_id());
    assert(o.get_depth() > 0.0);
  }
  // Test the == operator
  {
    tile a(0.0, 0.0, 0.0, 1, 2, 0.0, tile_type::grassland, tile_id());
    tile b(0.0, 0.0, 0.0, 1, 1, 0.0, tile_type::grassland, tile_id());
    tile c(0.0, 0.0, 0.0, 1, 1, 0.0, tile_type::grassland, tile_id());
    assert(!(a == b));
    assert(b == c);
  }

#define FIX_ISSUE_116_TILE_CONTAINS
#ifdef FIX_ISSUE_116_TILE_CONTAINS
  //
  //
  //   (0,0)------(100,0)
  //     |           |
  //     |     A     |     B
  //     |           |
  //   (0,100)-----(100,100)
  //
  //           C           D
  {
    const tile t(0.0, 0.0, 0.0, 1, 1, 0, tile_type::grassland, tile_id());
    assert(t.tile_contains(50, 50));   // A
    assert(!t.tile_contains(165, 50));  // B
    assert(!t.tile_contains(50, 165)); // C
    assert(!t.tile_contains(165, 165)); // D
  }
#endif // FIX_ISSUE_116_TILE_CONTAINS

  //#define FIX_ISSUE_246
  #ifdef FIX_ISSUE_246
  //
  //
  //   (0,0)------(100,0)
  //     |           |
  //     |     A     |     B
  //     |           |
  //   (0,100)-----(100,100)
  //
  //           C           D
  {
    const tile t(0.0, 0.0, 0.0, 1, 1, tile_type::grassland, 0);
    assert(contains(t, 50, 50));   // A
    assert(!contains(165, 50));  // B
    assert(!contains(50, 165)); // C
    assert(!contains(165, 165)); // D
  }
  #endif // FIX_ISSUE_246
  //operator==
  #ifdef FIX_OPERATOR_IS_IS
  {
    const tile a;

    // b with different dx
    tile b = a;
    assert(a == b);
    b.set_dx(a.get_dx() + 1.0);
    assert(!(a == b));

    // b with different dx
    b = a;
    assert(a == b);
    b.set_dy(a.get_dy() + 1.0);
    assert(!(a == b));
  }
  #endif
}
