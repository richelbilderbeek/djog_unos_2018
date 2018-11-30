#include "agent.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

agent::agent(const agent_type type, const double x, const double y)
    : m_type{type}, m_x{x}, m_y{y}{}

std::ostream& operator<<(std::ostream& os, const agent& a) noexcept
{
  //TODO: actually save the tile and agents
  os << a.m_type << ' ' << a.m_x << ' ' << a.m_y;
  return os;
}

std::istream& operator>>(std::istream& is, agent& a)
{
  //TODO: actually save the tile and agents
  is >> a.m_type >> a.m_x >> a.m_y;
  return is;
}

bool operator==(const agent& lhs, const agent& rhs) noexcept{
    if (!(lhs.m_type and rhs.m_type and lhs.m_x and rhs.m_x and lhs.m_y and rhs.m_y)))
       return false;
    return true;
}

void agent::move()
{
  if (m_type == agent_type::cow) {
    m_x += 0.1 * (-1 + (std::rand() % 3));
    m_y += 0.1 * (-1 + (std::rand() % 3));
  }
}

std::vector<agent> create_default_agents() noexcept //!OCLINT indeed too long
{
  std::vector<agent> agents;
  {
    agent a1(agent_type::cow);
    move_agent_to_tile(a1, 0, 0);
    agents.push_back(a1);
    agent a2(agent_type::cow, 40, 70);
    move_agent_to_tile(a2, 0, 0);
    agents.push_back(a2);
    agent a3(agent_type::grass, 70, 40);
    move_agent_to_tile(a3, 0, 0);
    agents.push_back(a3);
  }
  {
    agent a1(agent_type::cow);
    move_agent_to_tile(a1, 1, 0);
    agents.push_back(a1);
    agent a2(agent_type::cow, 90, 30);
    move_agent_to_tile(a2, 1, 0);
    agents.push_back(a2);
    agent a3(agent_type::cow, 30, 90);
    move_agent_to_tile(a3, 1, 0);
    agents.push_back(a3);
  }
  {
    agent a1(agent_type::crocodile, 30, 160);
    move_agent_to_tile(a1, 0, 2);
    agents.push_back(a1);
  }
  {
    agent a1(agent_type::crocodile);
    move_agent_to_tile(a1, 2, 1);
    agents.push_back(a1);
    agent a2(agent_type::grass);
    move_agent_to_tile(a2, 2, 1);
    agents.push_back(a2);
  }
  {
    agent a1(agent_type::fish);
    move_agent_to_tile(a1, 3, 2);
    agents.push_back(a1);
    agent a2(agent_type::fish, 10, 10);
    move_agent_to_tile(a2, 3, 2);
    agents.push_back(a2);
  }
  {
    agent a1(agent_type::grass);
    move_agent_to_tile(a1, 1, -1);
    agents.push_back(a1);
  }
  return agents;
}

void move_agent_to_tile(agent &a, double tile_x, double tile_y) {
  a.set_x(a.get_x()+(tile_x*115));
  a.set_y(a.get_y()+(tile_y*115));
}

void test_agent() //!OCLINT testing functions may be long
{
  // A default agent has coordinate (0,0)
  {
    const agent a(agent_type::cow);
    assert(a.get_x() == 0.0);
    assert(a.get_y() == 0.0);
  }
  // A agent has the right coordinats
  {
    const double x{12.34};
    const double y{56.78};
    const agent a(agent_type::cow, x, y);
    assert(a.get_x() == x);
    assert(a.get_y() == y);
  }
  // A cow moves
  {
    std::srand(314);
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::cow, x, y);
    a.move();
    assert(a.get_x() != x || a.get_y() != y);
  }
  //#define FIX_ISSUE_202
  #ifdef FIX_ISSUE_202
  // A crocodile moves
  {
    std::srand(15);
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::crocodile, x, y);
    for (int i = 0; i != 10; ++i) a.move(); //To make surer x or y is changed
    assert(a.get_x() != x || a.get_y() != y);
  }
<<<<<<< HEAD
  #endif // FIX_ISSUE_202

  //#define FIX_ISSUE_201
  #ifdef FIX_ISSUE_201
=======
>>>>>>> be4a9785279b7807e801203473416f9cccfb53ec
  // A fish moves
  {
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::fish, x, y);
    a.move();
    assert(a.get_x() != x || a.get_y() != y);
  }
  #endif // FIX_ISSUE_201
  // Grass does not move
  {
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::grass, x, y);
    a.move();
    assert(a.get_x() == x && a.get_y() == y);
  }
  //#define FIX_ISSUE_245
  #ifdef FIX_ISSUE_245
  // Agents have health
  {
    const agent a(agent_type::cow);
    assert(a.get_health() > 0.0);
  }
  #endif // FIX_ISSUE_245
}

