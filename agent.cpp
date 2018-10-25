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

void agent::move()
{
  if (m_type == agent_type::cow) {
    m_x += 0.1 * (-1 + (std::rand() % 3));
    m_y += 0.1 * (-1 + (std::rand() % 3));
  }
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
