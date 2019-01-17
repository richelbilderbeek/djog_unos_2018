#include "agent.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

#include "game.h"

agent::agent(const agent_type type, const double x, const double y, double health)
    : m_type{type}, m_x{x}, m_y{y}, m_health{health}, m_stamina{100}{}

std::ostream& operator<<(std::ostream& os, const agent& a) noexcept
{
  os << a.m_type << ' ' << a.m_x << ' ' << a.m_y << ' '
     << a.m_health << ' ' << a.m_stamina;
  return os;
}

std::istream& operator>>(std::istream& is, agent& a)
{
  is >> a.m_type >> a.m_x >> a.m_y >> a.m_health >> a.m_stamina;
  return is;
}

bool operator==(const agent& lhs, const agent& rhs) noexcept{
  return lhs.m_type == rhs.m_type and
         lhs.m_x == rhs.m_x and
         lhs.m_y == rhs.m_y and
         lhs.m_health == rhs.m_health;
  // TODO Stamina wasn't precise enough to compare, try again
}

// WARNING test this (line 256)
std::vector<agent_type> can_eat(const agent_type type) {
  switch (type) {
    case agent_type::crocodile:
      return {agent_type::cow};
    case agent_type::bird:
      return {agent_type::spider,
              agent_type::fish};
    case agent_type::cow:
      return {agent_type::grass};
    default:
      return {};
  }
}

// WARNING test this
void agent::eat(const game& g) {
  std::vector<agent_type> food = can_eat(m_type);
  //Is agent_type a in food?
  for (agent a : g.get_agents()) {
    if (a.get_x() > m_x - 50 &&
        a.get_x() < m_x + 60 &&
        a.get_y() > m_y - 50 &&
        a.get_y() < m_y + 55 &&
        std::count(std::begin(food), std::end(food), a.get_type()))
    {
      a.kill();
      m_stamina += 60;
    } else {
      m_stamina -= 0.05;
    }
  }
}

void agent::move(const game& g)
{
  //Dead agents stay still
  if (m_health <= 0.0) return;
  if (m_stamina <= 0.0) {
    m_health += (m_stamina - 1) * 0.2;
    eat(g);
    return;
  }

  if (m_type == agent_type::cow ||
      m_type == agent_type::crocodile ||
      m_type == agent_type::spider ||
      m_type == agent_type::goat ||
      m_type == agent_type::fish) {
    m_x += 0.1 * (-1 + (std::rand() % 3));
    m_y += 0.1 * (-1 + (std::rand() % 3));
  }

  if (g.get_n_ticks() % 100 == 0)
    eat(g);

  //TODO after fixing issue 261 uncomment this
  //Uncommenting this results in a crash...
  //if (!is_on_tile(g, *this))
  //{
  //  this->m_health = 0.0;
  //}
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
    agent a1(agent_type::fish);
    move_agent_to_tile(a1, 4, 2);
    agents.push_back(a1);
    agent a2(agent_type::fish, 10, 10);
    move_agent_to_tile(a2, 4, 2);
    agents.push_back(a2);
  }
  {
    agent a1(agent_type::grass);
    move_agent_to_tile(a1, 1, -1);
    agents.push_back(a1);
  }
  {
    agent a1(agent_type::bacterium);
    move_agent_to_tile(a1, 0, 0);
    agents.push_back(a1);
  }
  {
    agent a1(agent_type::tree, 10, 20);
    move_agent_to_tile(a1, 4, -1);
    agents.push_back(a1);
    agent a2(agent_type::tree, 40, 10);
    move_agent_to_tile(a2, 4, -1);
    agents.push_back(a2);
    agent a3(agent_type::tree, 50, 35);
    move_agent_to_tile(a3, 4, -1);
    agents.push_back(a3);
    agent a4(agent_type::tree, 60, 40);
    move_agent_to_tile(a4, 4, -1);
    agents.push_back(a4);
    agent a5(agent_type::tree, 35, 65);
    move_agent_to_tile(a5, 4, -1);
    agents.push_back(a5);
    agent a6(agent_type::tree);
    move_agent_to_tile(a6, 4, -1);
    agents.push_back(a6);
    agent a7(agent_type::spider, 40, 40);
    move_agent_to_tile(a7, 4, -1);
    agents.push_back(a7);
    agent a8(agent_type::bird, 45, 75);
    move_agent_to_tile(a8, 4, -1);
    agents.push_back(a8);
  }
  {
      agent a1(agent_type::goat, 190, 90);
      move_agent_to_tile(a1, 1, 2);
      agents.push_back(a1);
      agent a2(agent_type::goat, 50, 80);
      move_agent_to_tile(a2, 1, 2);
      agents.push_back(a2);
  }
  return agents;
}

void move_agent_to_tile(agent &a, double tile_x, double tile_y) {
  a.set_x(a.get_x()+(tile_x*115));
  a.set_y(a.get_y()+(tile_y*115));
}

bool agent::is_clicked(const double x, const double y,
                       const sf::Texture& sprite) const noexcept {
  return x > m_x - 5 &&
         x < m_x + sprite.getSize().x * 0.2 + 5 &&
         y > m_y - 5 &&
         y < m_y + sprite.getSize().y * 0.2 + 5;
}

void test_agent() //!OCLINT testing functions may be long
{
  // A default agent has coordinate (0,0)
  {
    const agent a(agent_type::cow);
    assert(a.get_x() == 0.0);
    assert(a.get_y() == 0.0);
  }
  // An agent has the right coordinats
  {
    const double x{12.34};
    const double y{56.78};
    const agent a(agent_type::cow, x, y);
    assert(a.get_x() == x);
    assert(a.get_y() == y);
  }
  // A cow moves
  {
    game g;
    std::srand(314);
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::cow, x, y);
    assert(is_on_tile(g, a));
    a.move(g);
    assert(a.get_x() != x || a.get_y() != y);
  }
  // A crocodile moves
  {
    game g;
    std::srand(15);
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::crocodile, x, y);
    for (int i = 0; i != 10; ++i) a.move(g); //To make surer x or y is changed
    assert(a.get_x() != x || a.get_y() != y);
  }
  // A fish moves
  {
    game g;
    std::srand(314);
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::fish, x, y);
    assert(is_on_tile(g, a));
    a.move(g);
    assert(a.get_x() != x || a.get_y() != y);
  }
  // Grass does not move
  {
    game g;
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::grass, x, y);
    assert(is_on_tile(g, a));
    a.move(g);
    assert(a.get_x() == x && a.get_y() == y);
  }
  // Agents have health
  {
    const agent a(agent_type::cow, 0, 0, 10);
    assert(a.get_health() > 0.0);
  }
  // Test can_eat
  {
    for (agent_type a : collect_all_agent_types()) {
      can_eat(a);
    }
  }
  //#define FIX_ISSUE_289
  #ifdef FIX_ISSUE_289
  //Agent can pass out of exhaustion
  {
    game g(create_default_tiles(), { agent(agent_type::cow) } );
    assert(!g.get_agents().empty());
    const auto stamina_before = g.get_agents()[0].get_stamina();
    // Exhaust one turn
    g.process_events();
    const auto stamina_after = g.get_agents()[0].get_stamina();
    assert(stamina_after < stamina_before);
  }
  #endif
  //A cow must starve if alone
  {
    game g(create_default_tiles(), { agent(agent_type::cow) } );
    assert(!g.get_agents().empty());
    //Exhaust cow
    while (g.get_agents()[0].get_stamina() > 0.0)
    {
      g.process_events();
    }
    const auto health_before = g.get_agents()[0].get_health();
    // Starve one turn
    g.process_events();
    const auto health_after = g.get_agents()[0].get_health();
    assert(health_after < health_before);
  }
  //#define FIX_ISSUE_285
  #ifdef FIX_ISSUE_285
  //An agent must be removed if health is below zero
  {
    game g(create_default_tiles(), { agent(agent_type::cow) } );
    assert(!g.get_agents().empty());
    // Wait until cow starves
    while (!g.get_agents().empty())
    {
      g.process_events();
    }
  }
  #endif
  //Grass grows
  {
    game g(create_default_tiles(), { agent(agent_type::grass) } );
    assert(!g.get_agents().empty());
    const auto health_before = g.get_agents()[0].get_health();
    // Grow one turn
    g.process_events();
    const auto health_after = g.get_agents()[0].get_health();
    assert(health_after > health_before);
  }

  //#define FIX_ISSUE_305
  #ifdef FIX_ISSUE_305
  //Trees grow
  {
    game g(create_default_tiles(), { agent(agent_type::tree) } );
    assert(!g.get_agents().empty());
    const auto health_before = g.get_agents()[0].get_health();
    // Grow one turn
    g.process_events();
    const auto health_after = g.get_agents()[0].get_health();
    assert(health_after > health_before);
  }
  #endif // FIX_ISSUE_305

  //#define FIX_ISSUE_303
  #ifdef FIX_ISSUE_303
  //Sessile agents that move on nothing get zero health
  {
    const std::vector<tile> no_tiles;
    game g(no_tiles, { agent(agent_type::crocodile, -100, -100, 100)});
    assert(g.get_agents()[0].get_health() > 0.0); //!OCLINT accepted idiom
    g.get_agents()[0].move(g);
    assert(g.get_agents()[0].get_health() == 0.0); //!OCLINT accepted idiom
  }
  #endif // FIX_ISSUE_303
  //#define FIX_ISSUE_300
  #ifdef FIX_ISSUE_300
  //Grass creates new grasses
  {
    game g(create_default_tiles(), { agent(agent_type::grass) } );
    assert(g.get_agents().size() == 1);
    while (g.get_agents().size() == 1) //Wait until grass procreates
    {
      g.process_events();
    }
    assert(g.get_agents()[0].get_type() == agent_type::grass);
    assert(g.get_agents()[1].get_type() == agent_type::grass);
  }
  #endif //FIX_ISSUE_300
  //Flying agent can fly over nothing without problems
  {
    const std::vector<tile> no_tiles;
    game g(no_tiles, { agent(agent_type::bird, -100, -100, 100)});
    assert(g.get_agents()[0].get_health() > 0.0); //!OCLINT accepted idiom
    g.get_agents()[0].move(g);
    assert(g.get_agents()[0].get_health() > 0.0); //!OCLINT accepted idiom
  }
  //#define FIX_ISSUE_301
  #ifdef FIX_ISSUE_301
  //Cows eat grass
  {
    const double cow_health{10.0};
    const double grass_health{5.0};
    game g(
      create_default_tiles(),
      {
        agent(agent_type::grass, 0.0, 0.0, grass_health),
        agent(agent_type::cow  , 0.0, 0.0, cow_health)
      }
    );
    assert(g.get_agents()[0].get_health() == grass_health);
    assert(g.get_agents()[1].get_health() == cow_health);
    g.process_events();
    //Grass is eaten ...
    assert(g.get_agents()[0].get_health() < grass_health);
    //Cow is fed ...
    assert(g.get_agents()[1].get_health() > cow_health);
  }
  #endif //FIX_ISSUE_301
}
