#include "tile.h"

#include <cassert>
#include <stdexcept>

tile::tile(
  const double x,
  const double y,
  const double width,
  const double height,
  const tile_type type
) : m_height{height},
    m_type{type},
    m_width{width},
    m_x{x},
    m_y{y}
{
  if (width <= 0.0)
  {
    throw std::invalid_argument("NO!");
  }

  assert(m_width > 0.0);
  assert(m_height > 0.0);
}

void test_tile() //!OCLINT testing function may be many lines
{
  #define FIX_ISSUE_85_TEST_TILE
  #ifdef FIX_ISSUE_85_TEST_TILE
  //width cannot be negative
  {
    try
    {
      const tile t(0.0, 0.0, -12.34, 100.0, tile_type::grassland);
      assert(t.get_width() < 0); //!OCLINT accepted idiom
    }
    catch (const std::invalid_argument& e)
    {
      assert(std::string(e.what()) == "'width' cannot be negative");
    }
  }
  //height cannot be negative
  {
    try
    {
      const tile t(0.0, 0.0, 100.0, -12.34, tile_type::grassland);
      assert(t.get_height() < 0); //!OCLINT accepted idiom
    }
    catch (const std::invalid_argument& e)
    {
      assert(std::string(e.what()) == "'height' cannot be negative");
    }
  }
  #endif // FIX_ISSUE_85_TEST_TILE


  //#define FIX_ISSUE_87_SET_TILE_SPEED
  #ifdef FIX_ISSUE_87_SET_TILE_SPEED
  //A tile starts from standstill
  {
    const tile t(0.0, 0.0, 10.0, 10.0, tile_type::grassland);
    assert(t.get_dx() == 0.0);
    assert(t.get_dy() == 0.0);
  }
  //Speed is set correctly
  {
    tile t(0.0, 0.0, 10.0, 10.0, tile_type::grassland);
    const double dx{12.34};
    const double dy{56.78};
    t.set_dx(dx);
    t.set_dy(dx);
    assert(t.get_dx() == dx);
    assert(t.get_dy() == dy);
  }
  //Tile responds to its speed
  {
    tile t(0.0, 0.0, 10.0, 10.0, tile_type::grassland);
    const double dx{12.34};
    const double dy{56.78};
    t.set_dx(dx);
    t.set_dy(dx);
    assert(t.get_x() != dx);
    assert(t.get_y() != dy);
    t.move();
    assert(t.get_x() == dx);
    assert(t.get_y() == dy);
  }
  #endif // FIX_ISSUE_87_SET_TILE_SPEED

  //#define FIX_ISSUE_92_ADD_AGENTS_ON_TILES
  #ifdef FIX_ISSUE_92_ADD_AGENTS_ON_TILES
  //A tile starts without agents
  {
    const tile t(0.0, 0.0, 10.0, 10.0, tile_type::grassland);
    const std::vector<agent>& agents = t.get_agents();
    assert(agents.size() == 0);
  }
  //Can add an agent to a tile
  {
    tile t(0.0, 0.0, 10.0, 10.0, tile_type::grassland);
    const agent a(agent_type::cow, 5.0, 5.0);
    t.add_agent(a);
    assert(t.get_agents().size() == 1);
  }
  #endif // FIX_ISSUE_92_ADD_AGENTS_ON_TILES
}
