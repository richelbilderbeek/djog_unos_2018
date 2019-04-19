#include "tile.h"
#include "agent.h"
#include "tile_id.h"
#include "tile_type.h"
#include "agent_type.h"
#include "sfml_game.h"
#include "game.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <tuple>

tile::tile(
  const double x,
  const double y,
  const double z,
  const double rotation,
  const double depth,
  const tile_type type,
  const tile_id id
) : m_depth{depth},
    m_dx{0.0},
    m_dy{0.0},
    m_dz{0.0},
    m_id{id},
    m_type{type},
    m_rotation{rotation},
    m_x{x},
    m_y{y},
    m_z{z}
{

  m_x *= 112;
  m_y *= 112;

}

std::vector<tile> create_test_default_tiles() noexcept //!OCLINT indeed a function that is too long
{
  std::vector<tile> tiles;
  {
    tile t(0, 0, 0, 90, 0, tile_type::grassland, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(1, 0, 1, 90, 0, tile_type::grassland, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(1, 5, 1, 0, 0, tile_type::grassland, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(0, 2, 2, 90, 0, tile_type::desert, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(2, 1, 3, 0, 0, tile_type::swamp, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(4, 4, 5, 0, 0, tile_type::dunes, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(1, 2, 4, 0, 0, tile_type::mountains, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(4, 1, 5, 0, 0, tile_type::arctic, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(3, 2, 6, 90, 20, tile_type::water, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(4, 2, 6, 90, 85, tile_type::water, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(5, 2, 8, 90, 0, tile_type::beach, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(6, 2, 8, 90, 0, tile_type::dunes, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(1, -1, 7, 0, 0, tile_type::savannah, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(4, -1, 8, 90, 0, tile_type::woods, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(0, -2, 8, 90, 0, tile_type::woods, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(6, -3, 8, 90, 0, tile_type::mangrove, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(-1, 0, 8, 90, 0, tile_type::rainforest, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(-2, 0, 8, 90, 0, tile_type::tundra, tile_id());
    tiles.push_back(t);
  }
  {
    tile t(-3, 0, 8, 90, 0, tile_type::hills, tile_id());
    tiles.push_back(t);
  }
  return tiles;
}

std::vector<tile> create_default_tiles() noexcept //!OCLINT indeed a function that is too long
{
  std::vector<tile> tiles;
  {tile t(2, 8, 0, 0, 0, tile_type::arctic, tile_id());tiles.push_back(t);}
  {tile t(4, 7, 0, 90, 0, tile_type::arctic, tile_id());tiles.push_back(t);}
  {tile t(5, 8, 0, 0, 0, tile_type::arctic, tile_id());tiles.push_back(t);}
  {tile t(1, 7, 0, 0, 0, tile_type::water, tile_id());tiles.push_back(t);}
  {tile t(5, 7, 0, 0, 0, tile_type::water, tile_id());tiles.push_back(t);}
  {tile t(7, 7, 0, 90, 0, tile_type::water, tile_id());tiles.push_back(t);}
  {tile t(8, 8, 0, 0, 0, tile_type::arctic, tile_id());tiles.push_back(t);}
  {tile t(8, 7, 0, 0, 0, tile_type::water, tile_id());tiles.push_back(t);}
  {tile t(2, 6, 0, 0, 0, tile_type::tundra, tile_id());tiles.push_back(t);}
  {tile t(4, 6, 0, 0, 0, tile_type::tundra, tile_id());tiles.push_back(t);}
  {tile t(6, 6, 0, 0, 0, tile_type::tundra, tile_id());tiles.push_back(t);}
  {tile t(9, 6, 0, 0, 0, tile_type::tundra, tile_id());tiles.push_back(t);}
  {tile t(2, 5, 0, 0, 0, tile_type::beach, tile_id());tiles.push_back(t);}
  {tile t(4, 5, 0, 0, 0, tile_type::beach, tile_id());tiles.push_back(t);}
  {tile t(6, 4, 0, 90, 0, tile_type::beach, tile_id());tiles.push_back(t);}
  {tile t(8, 5, 0, 0, 0, tile_type::beach, tile_id());tiles.push_back(t);}
  {tile t(2, 4, 0, 0, 0, tile_type::dunes, tile_id());tiles.push_back(t);}
  {tile t(4, 3, 0, 90, 0, tile_type::dunes, tile_id());tiles.push_back(t);}
  {tile t(7, 4, 0, 0, 0, tile_type::dunes, tile_id());tiles.push_back(t);}
  {tile t(2, 3, 0, 0, 0, tile_type::grassland, tile_id());tiles.push_back(t);}
  {tile t(5, 3, 0, 0, 0, tile_type::grassland, tile_id());tiles.push_back(t);}
  {tile t(7, 3, 0, 0, 0, tile_type::grassland, tile_id());tiles.push_back(t);}
  {tile t(9, 3, 0, 0, 0, tile_type::grassland, tile_id());tiles.push_back(t);}
  {tile t(2, 2, 0, 0, 0, tile_type::woods, tile_id()); tiles.push_back(t);}
  {tile t(4, 1, 0, 90, 0, tile_type::woods, tile_id());tiles.push_back(t);}
  {tile t(5, 2, 0, 0, 0, tile_type::woods, tile_id());tiles.push_back(t);}
  {tile t(7, 2, 0, 0, 0, tile_type::woods, tile_id());tiles.push_back(t);}
  {tile t(9, 2, 0, 0, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(2, 1, 0, 0, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(5, 0, 0, 90, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(6, 1, 0, 0, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(8, 1, 0, 0, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(3, 0, 0, 0, 0, tile_type::rainforest, tile_id());tiles.push_back(t);}
  {tile t(6, -1, 0, 90, 0, tile_type::rainforest, tile_id());tiles.push_back(t);}
  {tile t(7, 0, 0, 0, 0, tile_type::rainforest, tile_id());tiles.push_back(t);}
  {tile t(9, 0, 0, 0, 0, tile_type::rainforest, tile_id());tiles.push_back(t);}
  {tile t(2, -1, 0, 0, 0, tile_type::savannah, tile_id());tiles.push_back(t);}
  {tile t(4, -1, 0, 0, 0, tile_type::savannah, tile_id());tiles.push_back(t);}
  {tile t(8, -1, 0, 0, 0, tile_type::savannah, tile_id());tiles.push_back(t);}
  {tile t(8, -2, 0, 0, 0, tile_type::savannah, tile_id());tiles.push_back(t);}
  {tile t(7, -2, 0, 90, 0, tile_type::savannah, tile_id());tiles.push_back(t);}
  {tile t(4, -2, 0, 0, 0, tile_type::savannah, tile_id());tiles.push_back(t);}
  {tile t(2, -2, 0, 0, 0, tile_type::savannah, tile_id());tiles.push_back(t);}
  {tile t(9, -3, 0, 0, 0, tile_type::rainforest, tile_id());tiles.push_back(t);}
  {tile t(7, -3, 0, 0, 0, tile_type::rainforest, tile_id());tiles.push_back(t);}
  {tile t(6, -3, 0, 90, 0, tile_type::rainforest, tile_id());tiles.push_back(t);}
  {tile t(3, -3, 0, 0, 0, tile_type::rainforest, tile_id());tiles.push_back(t);}
  {tile t(8, -4, 0, 0, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(6, -4, 0, 0, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(5, -4, 0, 90, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(2, -4, 0, 0, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(9, -4, 0, 0, 0, tile_type::mangrove, tile_id());tiles.push_back(t);}
  {tile t(7, -5, 0, 0, 0, tile_type::woods, tile_id());tiles.push_back(t);}
  {tile t(5, -5, 0, 0, 0, tile_type::woods, tile_id());tiles.push_back(t);}
  {tile t(4, -5, 0, 90, 0, tile_type::woods, tile_id());tiles.push_back(t);}
  {tile t(2, -5, 0, 0, 0, tile_type::woods, tile_id()); tiles.push_back(t);}
  {tile t(9, -6, 0, 0, 0, tile_type::grassland, tile_id());tiles.push_back(t);}
  {tile t(7, -6, 0, 0, 0, tile_type::grassland, tile_id());tiles.push_back(t);}
  {tile t(5, -6, 0, 0, 0, tile_type::grassland, tile_id());tiles.push_back(t);}
  {tile t(2, -6, 0, 0, 0, tile_type::grassland, tile_id());tiles.push_back(t);}
  {tile t(7, -7, 0, 0, 0, tile_type::dunes, tile_id());tiles.push_back(t);}
  {tile t(4, -7, 0, 90, 0, tile_type::dunes, tile_id());tiles.push_back(t);}
  {tile t(2, -7, 0, 0, 0, tile_type::dunes, tile_id());tiles.push_back(t);}
  {tile t(8, -8, 0, 0, 0, tile_type::beach, tile_id());tiles.push_back(t);}
  {tile t(6, -8, 0, 90, 0, tile_type::beach, tile_id());tiles.push_back(t);}
  {tile t(4, -8, 0, 0, 0, tile_type::beach, tile_id());tiles.push_back(t);}
  {tile t(2, -8, 0, 0, 0, tile_type::beach, tile_id());tiles.push_back(t);}
  {tile t(9, -9, 0, 0, 0, tile_type::tundra, tile_id());tiles.push_back(t);}
  {tile t(6, -9, 0, 0, 0, tile_type::tundra, tile_id());tiles.push_back(t);}
  {tile t(4, -9, 0, 0, 0, tile_type::tundra, tile_id());tiles.push_back(t);}
  {tile t(2, -9, 0, 0, 0, tile_type::tundra, tile_id());tiles.push_back(t);}
  {tile t(8, -10, 0, 0, 0, tile_type::water, tile_id());tiles.push_back(t);}
  {tile t(8, -11, 0, 0, 0, tile_type::arctic, tile_id());tiles.push_back(t);}
  {tile t(7, -11, 0, 90, 0, tile_type::water, tile_id());tiles.push_back(t);}
  {tile t(5, -10, 0, 0, 0, tile_type::water, tile_id());tiles.push_back(t);}
  {tile t(1, -10, 0, 0, 0, tile_type::water, tile_id());tiles.push_back(t);}
  {tile t(5, -11, 0, 0, 0, tile_type::arctic, tile_id());tiles.push_back(t);}
  {tile t(4, -11, 0, 90, 0, tile_type::arctic, tile_id());tiles.push_back(t);}
  {tile t(2, -11, 0, 0, 0, tile_type::arctic, tile_id());tiles.push_back(t);}
  return tiles;
}

tile create_test_tile() noexcept
{
  return tile(0.0, 0.0, 0.0, 0.0, 0.0, tile_type::grassland);
}

std::vector<tile> create_two_grass_tiles() noexcept
{
  return
  {
    //   x    y    z   w    h    d   type               ID
    tile(1, 1, 1, 0, 0, tile_type::grassland, tile_id()),
    tile(3, 1, 1, 0, 0, tile_type::grassland, tile_id())
  };
}

bool have_same_position(const tile& lhs, const tile& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && lhs.get_y() == rhs.get_y();
}

void tile::process_events(game& g) //!OCLINT high cyclomatic complexity
{
  if (!g.allow_spawning() || m_type == tile_type::arctic) return;
  //Spawning
  //Land type, common agent type spawned, rare agent type spawned
  using triplet = std::tuple<tile_type, agent_type, agent_type>;
  const std::vector<triplet> v =
  {
    triplet(tile_type::arctic, agent_type::none, agent_type::none),
    triplet(tile_type::beach, agent_type::none, agent_type::squirrel),
    triplet(tile_type::desert, agent_type::cactus, agent_type::snake),
    triplet(tile_type::dunes, agent_type::grass, agent_type::squirrel),
    triplet(tile_type::grassland, agent_type::grass, agent_type::cow),
    triplet(tile_type::hills, agent_type::foxgloves, agent_type::goat),
    triplet(tile_type::mangrove, agent_type::worm, agent_type::crocodile),
    triplet(tile_type::mountains, agent_type::foxgloves, agent_type::bird),
    triplet(tile_type::rainforest, agent_type::tree, agent_type::cow),
    triplet(tile_type::savannah, agent_type::grass, agent_type::giraffe),
    triplet(tile_type::swamp, agent_type::venus_fly_trap, agent_type::cow),
    triplet(tile_type::tundra, agent_type::grass, agent_type::cow),
    triplet(tile_type::water, agent_type::plankton, agent_type::fish),
    triplet(tile_type::woods, agent_type::tree, agent_type::spider)
  };

  if(g.get_n_ticks() % ticks == 0){
    ticks = std::rand() % ((1800 - 1400) + 1) + 1400;
    const auto here = std::find_if(
      std::begin(v),
      std::end(v),
      [this](const triplet& t)
      {
        return std::get<0>(t) == m_type;
      }
    );
    assert(here != std::end(v));
    if ((std::rand() >> 4) % 10 == 0) {
      //rare
      spawn(g, std::get<2>(*here));
    } else {
      //common
      spawn(g, std::get<1>(*here));
    }
  }
}

void tile::rotate_c() { //TODO
  // if rotation is 180 reset it to 0 and move tile
  /*const double width = t.get_width();
  const double height = t.get_height();
  t.set_width(height);
  t.set_height(width);*/
}

void tile::rotate_cc() { //TODO
  // if rotation is 0 change to 180 and move tile
  // reset rotation from ^^^
}

double tile::get_width() const {
  return (static_cast<int>(m_rotation / 90.0)) % 2 == 1 ? 100 : 212;
}

double tile::get_height() const {
  return (static_cast<int>(m_rotation / 90.0)) % 2 == 1 ? 212 : 100;
}

void tile::spawn(game& g, agent_type type) { //!OCLINT high cyclomatic complexity
  const double max_distance_x{get_width()};
  const double max_distance_y{get_height() - 40};
  double f_x{static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)};
  double f_y{static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)};
  assert(f_x >= 0.0 && f_x < 1.0);
  assert(f_y >= 0.0 && f_y < 1.0);
  double new_x{m_x + (((f_x * 2.0) - 1.0) * max_distance_x)};
  double new_y{m_y + (((f_y * 2.0) - 1.0) * max_distance_y)};
  agent new_agent(type, new_x, new_y, 100, 0, can_eat(type));
  tile t = *this;
  while(!is_on_tile(g, new_agent)
        || !is_on_specific_tile(new_agent.get_x() - 6, new_agent.get_y() - 6, t)
        || !is_on_specific_tile(new_agent.get_x() + 18, new_agent.get_y() + 18, t)){
    f_x = static_cast<double>(std::rand()) / (1.0 + static_cast<double>(RAND_MAX));
    f_y = static_cast<double>(std::rand()) / (1.0 + static_cast<double>(RAND_MAX));
    assert(f_x >= 0.0 && f_x < 1.0);
    assert(f_y >= 0.0 && f_y < 1.0);
    new_x = m_x + (((f_x * 2.0) - 1.0) * max_distance_x);
    new_y = m_y + (((f_y * 2.0) - 1.0) * max_distance_y);
    new_agent.set_x(new_x);
    new_agent.set_y(new_y);
  }
  g.add_agents( { new_agent } );
}

void tile::set_dx(double dx) {
  if (!m_locked)
    m_dx = dx;
}

void tile::set_dy(double dy) {
  if (!m_locked)
    m_dy = dy;
}

void tile::set_rotation(double r) {
  m_rotation = r;
}

void tile::set_type(const tile_type t) noexcept
{
  m_type = t;
}

void tile::move(std::vector<agent>& a) {
  m_x += m_dx;
  m_y += m_dy;
  m_z += m_dz;

  for (auto& agent: a) {
      if(is_on_specific_tile(agent, *this)){
        agent.move(m_dx, m_dy);
      }
  }
}

void tile::move() {
  m_x += m_dx;
  m_y += m_dy;
  m_z += m_dz;
}

std::ostream& operator<<(std::ostream& os, const tile& t)
{
  os << t.m_x << ' ' << t.m_y << ' '
     << t.m_rotation << ' '
     << t.m_locked << ' ' << t.m_type << ' '
     << t.m_dx << ' ' << t.m_dy << ' '
     << t.m_depth;

  return os;
}

std::istream& operator>>(std::istream& is, tile& t)
{
  is >> t.m_x >> t.m_y;
  is >> t.m_rotation;
  is >> t.m_locked;
  is >> t.m_type;
  is >> t.m_dx >> t.m_dy;
  is >> t.m_depth;
  return is;
}

bool operator==(const tile& lhs, const tile& rhs) noexcept {
  return
       lhs.m_depth    == rhs.m_depth
    && lhs.m_dx       == rhs.m_dx
    && lhs.m_dy       == rhs.m_dy
    && lhs.m_dz       == rhs.m_dz
    && lhs.m_locked   == rhs.m_locked
    && lhs.m_rotation == rhs.m_rotation
    && lhs.m_type     == rhs.m_type
    && lhs.m_x        == rhs.m_x
    && lhs.m_y        == rhs.m_y
    && lhs.m_z        == rhs.m_z
  ;
}

bool contains(const tile& t, double x, double y) noexcept {
  return x > t.get_x() - 5 && x < t.get_x() + t.get_width() + 5 && y > t.get_y() - 5 &&
         y < t.get_y() + t.get_height() + 5;
}

void tile::lock_movement(bool b) { m_locked = b; }

void test_tile() //!OCLINT testing function may be many lines
{
  //#define FIX_ISSUE_522
  #ifdef FIX_ISSUE_522
  //A tile behaves as expected
  {
    const double x{1.0};
    const double y{2.0};
    const double z{3.0};
    const double width{4.0};
    const double height{5.0};
    const double depth{6.0};
    const tile_type type{tile_type::arctic};
    const tile_id id{tile_id()};

    const tile a(x, y, z, width, height, depth, type, id);
    assert(std::abs(x - a.get_x()) < 0.001);
    assert(std::abs(y - a.get_y()) < 0.001);
    assert(std::abs(z - a.get_z()) < 0.001);
    assert(std::abs(width - a.get_width()) < 0.001);
    assert(std::abs(height - a.get_height()) < 0.001);
    assert(std::abs(depth - a.get_depth()) < 0.001);
    assert(type == a.get_type());
    assert(id.get() == a.get_id());
  }
  #endif // FIX_ISSUE_522
  // A tile starts from standstill
  {
    const tile t(0.0, 0.0, 0.0, 0.0, 0.0, tile_type::grassland, tile_id());
    assert(t.get_dx() == 0.0);
    assert(t.get_dy() == 0.0);
  }
  {
    tile t(0.0, 0.0, 0.0, 0.0, 0.0, tile_type::grassland, tile_id());
    t.lock_movement(true);
    t.set_dx(12.34);
    t.set_dy(56.78);
    assert(t.get_dx() == 0.0);
    assert(t.get_dy() == 0.0);
  }
  // Speed is set correctly
  {
    tile t(0.0, 0.0, 0.0, 0.0, 0.0, tile_type::grassland, tile_id());
    const double dx{12.34};
    const double dy{56.78};
    t.set_dx(dx);
    t.set_dy(dy);
    assert(t.get_dx() == dx);
    assert(t.get_dy() == dy);
  }
  // Tile responds to its speed
  {
    tile t(0.0, 0.0, 0.0, 0.0, 0.0, tile_type::grassland, tile_id());
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
    tile g(0.0, 0.0, 0.0, 0.0, 0.0, tile_type::grassland, tile_id());
    assert(g.get_depth() == 0.0);
    tile o(0.0, 0.0, 0.0, 0.0, 1.0, tile_type::water, tile_id());
    assert(o.get_depth() > 0.0);
  }
  // Test the == operator
  {
    const tile a(0.0, 0.0, 0.0,  0.0, 0.0, tile_type::grassland, tile_id());
    const tile b(0.0, 0.0, 0.0, 90.0, 0.0, tile_type::grassland, tile_id());
    const tile c(0.0, 0.0, 0.0, 90.0, 0.0, tile_type::grassland, tile_id());
    assert(!(a == b));
    assert(b == c);
  }

  //#define FIX_ISSUE_246
  #ifdef FIX_ISSUE_246
  //Depends on #522
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
    const double width{100.0};
    const double height{100.0};
    const tile t(0.0, 0.0, 0.0, width, height);
    assert(t.get_width() == width); // #522
    assert(t.get_height() == height); // #522
    assert( contains(t, 1.0 * width / 2.0, 1.0 * height / 2.0));  // A
    assert(!contains(t, 3.0 * width / 2.0, 1.0 * height / 2.0));  // B
    assert(!contains(t, 1.0 * width / 2.0, 3.0 * height / 2.0));  // C
    assert(!contains(t, 3.0 * width / 2.0, 3.0 * height / 2.0));  // D
  }
  #endif // FIX_ISSUE_246
  //operator==
  {
    const tile a = create_test_tile();

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
  {
    assert(create_two_grass_tiles().size() == 2);
  }
  //#define FIX_ISSUE_463
  #ifdef FIX_ISSUE_463
  //Depends on #522
  //A tile can rotate, #463
  {
    const double width{4.5};
    const double height{5.6};
    tile a(1.2, 2.3, 3.4, width, height);
    assert(width == a.get_width());
    assert(height == a.get_height());
    rotate(a);
    assert(height == a.get_width());
    assert(width == a.get_height());
    assert(1==2);
  }
  #endif //FIX_ISSUE_463
}
