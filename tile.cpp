#include "tile.h"
#include "agent.h"
#include "id.h"
#include "tile_type.h"
#include "agent_type.h"
#include "sfml_game.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

tile::tile(const double x, const double y, const double z, const double width,
           const double height, const tile_type type, const int id)
    : m_height{height}, m_type{type}, m_width{width}, m_x{x}, m_y{y}, m_z{z}, m_id{id}
{
  m_dx = 0;
  m_dy = 0;
  m_dz = 0;

  if (width <= 0.0)
  {
    throw std::invalid_argument("'width' cannot be negative");
  }

  if (height <= 0.0)
  {
    throw std::invalid_argument("'height' cannot be negative");
  }

  assert(m_width > 0.0);
  assert(m_height > 0.0);


  if (m_type == tile_type::ocean)
  {
    m_agents.emplace_back(agent(agent_type::fish, width / 2.0, height / 2.0));
  } else {
    m_agents.emplace_back(agent(agent_type::cow, width / 2.0, height / 2.0));
  }
}

std::vector<tile> create_default_tiles() noexcept
{
  std::vector<tile> tiles;
  {
    tile t(100, 100, 10, 215, 100, tile_type::grassland, new_id());
    tiles.push_back(t);
  }
  {
    tile t(-15, 330, 15, 215, 100, tile_type::grassland, new_id());
    tiles.push_back(t);
  }
  {
    tile t(215, 215, 20, 100, 215, tile_type::mountains, new_id());
    tiles.push_back(t);
  }
  {
    tile t(-15, 215, 30, 215, 100, tile_type::ocean, new_id());
    tiles.push_back(t);
  }
  {
    tile t(-15, -15, 40, 215, 100, tile_type::arctic, new_id());
    tiles.push_back(t);
  }
  {
    tile t(215, -15, 50, 215, 100, tile_type::savannah, new_id());
    tiles.push_back(t);
  }
  {
    tile t(445, -15, 60, 100, 215, tile_type::desert, new_id());
    tiles.push_back(t);
  }
  return tiles;
}

void tile::process_events()
{
  for (auto& a: m_agents) {
    a.move();
  }

}

void tile::set_dx(double dx) {
  if (!m_locked)
    m_dx = dx;
}

void tile::set_dy(double dy) {
  if (!m_locked)
    m_dy = dy;
}

void tile::set_dz(double dz) {
  if(!m_locked)
    m_dz = dz;
}

void tile::move() {
  m_x += m_dx;
  m_y += m_dy;
  m_z += m_dz;
}

std::ostream& operator<<(std::ostream& os, const tile& t)
{
  //TODO: actually save the tile and agents
  os << t.m_x << ' ' << t.m_y << ' '
     << t.m_height << ' ' << t.m_width << ' '
     << t.m_locked << ' ' << t.m_type << ' '
     << t.m_dx << ' ' << t.m_dy
     << t.m_agents.size() << ' ';

  for (int i=0; i < static_cast<int>(t.m_agents.size()); i++){
      os << t.m_agents[i];
  }

  return os;
}

std::istream& operator>>(std::istream& is, tile& t)
{
  //TODO: actually save the tile and agents
  is >> t.m_x >> t.m_y;
  is >> t.m_height >> t.m_width;
  is >> t.m_locked >> t.m_type;
  is >> t.m_dx >> t.m_dy;
  int n_agents = 1;
  is >> n_agents;
  //TODO: the line below is a stub
  for (int i=0; i!=n_agents; ++i)
  {
    agent a(agent_type::none, 0, 0);
    is >> a;
    t.m_agents.emplace_back(
      a
    );
  }
  return is;
}


void tile::add_agent(agent a) { m_agents.push_back(a); }

void tile::set_id(int id) { m_id = id; }

bool tile::tile_contains(double x, double y) const noexcept {
  return x > m_x - 5 && x < m_x + m_width + 5 && y > m_y - 5 &&
         y < m_y + m_height + 5;
}

void tile::lock_movement(bool b) { m_locked = b; }

void test_tile() //!OCLINT testing function may be many lines
{
#define FIX_ISSUE_85_TEST_TILE
#ifdef FIX_ISSUE_85_TEST_TILE
  // width cannot be negative
  {
    try {
      const tile t(0.0, 0.0, 0.0, -12.34, 100.0, tile_type::grassland, //!OCLINT indeed t is unused
                   0);
      assert(!"This should not be executed"); //!OCLINT accepted idiom
    } catch (const std::invalid_argument &e) {
      assert(std::string(e.what()) == "'width' cannot be negative");
    }
  }
  // height cannot be negative
  {
    try {
      const tile t(0.0, 0.0, 0.0, 100.0, -12.34, tile_type::grassland, //!OCLINT indeed t is unused
                   0);                        //!OCLINT accepted idiom
      assert(!"This should not be executed"); //!OCLINT accepted idiom
    } catch (const std::invalid_argument &e) {
      assert(std::string(e.what()) == "'height' cannot be negative");
    }
  }
#endif // FIX_ISSUE_85_TEST_TILE

#define FIX_ISSUE_87_SET_TILE_SPEED
#ifdef FIX_ISSUE_87_SET_TILE_SPEED
  // A tile starts from standstill
  {
    const tile t(0.0, 0.0, 0.0, 10.0, 10.0, tile_type::grassland, 0);
    assert(t.get_dx() == 0.0);
    assert(t.get_dy() == 0.0);
  }
  // Speed is set correctly
  {
    tile t(0.0, 0.0, 0.0, 10.0, 10.0, tile_type::grassland, 0);
    const double dx{12.34};
    const double dy{56.78};
    t.set_dx(dx);
    t.set_dy(dy);
    assert(t.get_dx() == dx);
    assert(t.get_dy() == dy);
  }
  // Tile responds to its speed
  {
    tile t(0.0, 0.0, 0.0, 10.0, 10.0, tile_type::grassland, 0);
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
#endif // FIX_ISSUE_87_SET_TILE_SPEED

  // A tile starts with one agent
  {
    const tile t(0.0, 0.0, 0.0, 10.0, 10.0, tile_type::grassland, 0);
    const std::vector<agent> &agents = t.get_agents();
    assert(agents.size() == 1);
  }
  // Can add an agent to a tile
  {
    tile t(0.0, 0.0, 0.0, 10.0, 10.0, tile_type::grassland, 0);
    const agent a(agent_type::cow, 5.0, 5.0);
    t.add_agent(a);
    assert(t.get_agents().size() == 2);
  }

#define FIX_ISSUE_116_TILE_CONTAINS
#ifdef FIX_ISSUE_116_TILE_CONTAINS
  //
  //
  //   (10,0)------(30,0)
  //     |           |
  //     |     A     |     B
  //     |           |
  //   (10,10)-----(30,10)
  //
  //           C           D
  {
    const tile t(10.0, 0.0, 0.0, 20.0, 10.0, tile_type::grassland, 0);
    assert(t.tile_contains(20, 5));   // A
    assert(!t.tile_contains(40, 5));  // B
    assert(!t.tile_contains(20, 15)); // C
    assert(!t.tile_contains(40, 15)); // D
  }
#endif // FIX_ISSUE_116_TILE_CONTAINS
}
