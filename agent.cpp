#include "agent.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

agent::agent(const agent_type type, const double x, const double y,
             const Texture texture)
    : m_type{type}, m_x{x}, m_y{y}, m_texture{texture} {
  m_sprite.setTexture(texture);
}

Sprite agent::getSprite() { return m_sprite; }
Texture agent::getTexure() { return m_texture; }

std::ostream& operator<<(std::ostream& os, const agent& a)
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
}
