#include "agent.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

#include "agent_type.h"
#include "game.h"

using namespace sf;

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

void agent::eat(const game& g) {
  std::vector<agent_type> food = can_eat(m_type);
  //Is agent_type a in food?
  for (agent a : g.get_agents()) {
    // NOTE not calculated from the center of the agent
    if (is_in_range(a.get_x(),
                    a.get_y(),
                    50.0) &&
        a.get_health() > 0 &&
        std::count(std::begin(food), std::end(food), a.get_type()))
    {
      m_stamina += 1;
    } else if (!(m_type == agent_type::grass ||
                 m_type == agent_type::tree)){
      m_stamina -= 0.05;
    }
    std::vector<agent_type> a_food = can_eat(a.get_type());
    // NOTE not calculated from the center of the agent
    if (is_in_range(a.get_x(),
                    a.get_y(),
                    50.0) &&
        m_health > 0 &&
        std::count(std::begin(a_food), std::end(a_food), m_type))
    {
      m_stamina = 0;
      m_health -= 0.1;
    }
  }
}

bool agent::is_in_range(double x, double y, double range) {
  // NOTE not calculated from the center of the agent
  // sfml_resources::get().get_agent_sprite(*this).getSize() / 2
  return x > m_x - range &&
         x < m_x + range &&
         y > m_y - range &&
         y < m_y + range;
}

void agent::move()
{
  //Dead agents stay still
  if (m_health <= 0.0) return;
  if (m_stamina <= 0.0) {
    m_health += (m_stamina - 1) * 0.2;
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
}

void agent::move(double dx, double dy) {
  m_x += dx;
  m_y += dy;
}

void agent::move_with_tile(){
  m_x += m_dx;
  m_y += m_dy;
}

void agent::process_events(game& g) {
  move();

  if(m_type == agent_type::grass ||
     m_type == agent_type::tree)
  {
    m_health += 0.001;

    if (m_health > 1000000.0)
    {
      const agent new_grass(
        agent_type::grass,
        m_x - 1.0 + static_cast<double>(std::rand() % 3),
        m_y - 1.0 + static_cast<double>(std::rand() % 3),
        m_health / 2.0
      );
      const std::vector<agent> agents( { new_grass } );
      g.add_agents(agents);
      m_health = m_health / 2.0;
    }
  }

  if (g.get_n_ticks() % 100 == 0)
    eat(g);

  if(m_type != agent_type::bird)
  {
    if (!is_on_tile(g, *this))
    {
      m_health = 0;
    }
  }

  if(m_dx != 0 || m_dy != 0){
    move_with_tile();
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
                       const Texture& sprite) const noexcept {
  return x > m_x - 5 &&
         x < m_x + sprite.getSize().x * 0.2 + 5 &&
         y > m_y - 5 &&
         y < m_y + sprite.getSize().y * 0.2 + 5;
}

sf::Vector2f agent::get_center(const sf::Texture &sprite) const {
  return sf::Vector2f(m_x + sprite.getSize().x * 0.2 / 2.0f,
                      m_y + sprite.getSize().y * 0.2 / 2.0f);
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
    a.move();
    assert(a.get_x() != x || a.get_y() != y);
  }
  // A crocodile moves
  {
    //game g; // TODO add assert is on tile
    std::srand(15);
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::crocodile, x, y);
    for (int i = 0; i != 10; ++i) a.move(); //To make surer x or y is changed
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
    a.move();
    assert(a.get_x() != x || a.get_y() != y);
  }
  // Grass does not move
  {
    game g;
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::grass, x, y);
    assert(is_on_tile(g, a));
    a.move();
    assert(a.get_x() == x && a.get_y() == y);
  }
  // Agents have health
  {
    const agent a(agent_type::cow, 0, 0, 10);
    assert(a.get_health() > 0.0);
  }
  //#define FIX_ISSUE_325
  #ifdef FIX_ISSUE_325
  // Agents have a direction, that can be read
  {
    const agent a(agent_type::cow); //Must be const
    assert(a.get_direction() == 0.0);
  }
  // Agents have a direction, that can be set
  {
    agent a(agent_type::cow);
    a.set_direction(3.14);
    assert(a.get_direction() == 3.14);
  }
  #endif // FIX_ISSUE_325
  // Test can_eat
  {
    for (agent_type a : collect_all_agent_types()) {
      can_eat(a);
    }
  }
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
  //A cow must starve if alone
  {
    game g({ tile(-1, -1, 0, 2, 2) }, { agent(agent_type::cow) } );
    assert(!g.get_agents().empty());
    const auto health_before = g.get_agents()[0].get_health();
    //Exhaust cow
    while (g.get_agents()[0].get_stamina() > 0.0)
    {
      g.get_agents()[0].eat(g);
    }
    // Starve one turn
    g.process_events();
    const auto health_after = g.get_agents()[0].get_health();
    assert(health_after < health_before);
  }
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
  //Sessile agents that move on nothing get zero health
  {
    const std::vector<tile> no_tiles;
    game g(no_tiles, { agent(agent_type::crocodile, -100, -100, 100)});
    assert(g.get_agents()[0].get_health() > 0.0); //!OCLINT accepted idiom
    g.process_events();
    assert(g.get_agents()[0].get_health() == 0.0); //!OCLINT accepted idiom
  }
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
    g.get_agents()[0].process_events(g);
    assert(g.get_agents()[0].get_health() > 0.0); //!OCLINT accepted idiom
  }
  //Cows eat grass
  {
    const double grass_health{5.0};
    game g(
      create_default_tiles(),
      {
        agent(agent_type::grass, 0.0, 0.0, grass_health),
        agent(agent_type::cow  , 0.0, 0.0, 10.0)
      }
    );
    assert(g.get_agents()[0].get_health() == grass_health);
    double cow_stamina = g.get_agents()[1].get_stamina();
    g.process_events();
    //Grass is eaten ...
    assert(g.get_agents()[0].get_health() < grass_health);
    //Cow is fed ...
    assert(g.get_agents()[1].get_stamina() > cow_stamina);
  }
}
