#include "agent.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <set>
#include <random>
#include <chrono>
#include "agent_type.h"
#include "biology.h"
#include "game.h"

using namespace sf;

agent::agent(
  const agent_type type,
  const double x,
  const double y,
  const double health,
  const double direction,
  std::vector<agent_type> prey
) : m_type{type},
    m_x{x},
    m_y{y},
    m_direction{direction},
    m_health{health},
    m_stamina{100},
    m_prey{prey}
{
  //An agent of type none is used when a 'null' agent is needed
}

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
  return
    lhs.m_type == rhs.m_type &&
    lhs.m_x == rhs.m_x &&
    lhs.m_y == rhs.m_y &&
    lhs.m_health == rhs.m_health &&
    lhs.m_stamina == rhs.m_stamina
  ;
}

double get_agent_reproduction_health(const agent_type t) noexcept
{
  switch(t)
  {
    case (agent_type::tree):
      return 500.0;
    default:
      return 100.0;
  }
}

double pythagoras(double x_length, double y_length)
{
  return sqrt((x_length * x_length) + (y_length * y_length));
}

std::vector<agent_type> can_eat(const agent_type type) {
  switch (type) {
    case agent_type::chameleon:
      return {agent_type::worm,
              agent_type::spider,
              agent_type::bird};
    case agent_type::crocodile:
      return {agent_type::cow,
              agent_type::giraffe,
              agent_type::fish,
              agent_type::goat,
              agent_type::squirrel};
    case agent_type::polar_bear:
      return {agent_type::fish};
    case agent_type::squirrel:
      return {agent_type::tree};
    case agent_type::whale:
      return {agent_type::fish};
    case agent_type::snake:
      return {agent_type::squirrel};
    case agent_type::venus_fly_trap:
      return {agent_type::spider,
              agent_type::worm};
    case agent_type::bird:
      return {agent_type::spider,
              agent_type::fish,
              agent_type::snake,
              agent_type::worm};
    case agent_type::cow:
      return {agent_type::grass};
    case agent_type::lion:
      return {agent_type::cow,
              agent_type::giraffe};
    case agent_type::giraffe:
      return {agent_type::tree};
    //case agent_type::venus_fly_trap:
    //  return {agent_type::spider };
    default:
      return {};
  }
}

bool is_plant(const agent_type type) noexcept {
  //Some plankton are also bacteria, archea, protozoa or animals
  return type == agent_type::plankton ||
         type == agent_type::grass ||
         type == agent_type::tree ||
         type == agent_type::cactus ||
         type == agent_type::foxgloves ||
         type == agent_type::venus_fly_trap ||
         type == agent_type::sunflower;
}

void agent::eat(game& g) { //!OCLINT high compexity

  //Plants do not eat
  if (is_plant(m_type)) return;

  //Is agent_type a in food?
  for (agent& other : g.get_agents()) {
    //Agents never eat themselves
    if (this == &other) continue;

    // Focal agent will not eat corpses
    if (other.get_health() <= 0.0) continue;

    //Skip other agent if it is not in range
    // NOTE not calculated from the center of the agent
    if (!is_in_range(other.get_x(), other.get_y(), 25.0)) continue;

    //Skip other agent if it is not a prey type
    const agent_type prey_type = other.get_type();
    if (std::count(std::begin(m_prey), std::end(m_prey), prey_type) == 0) continue;

    // Focal agent will eat the prey
    // As in any food chain, energy is lost: the predator gains less energy
    // than the prey gains
    m_health += biology().get_health_increase_when_eating();
    m_stamina += biology().get_stamina_increase_when_eating();
    other.set_health(other.get_health() - biology().get_health_decrease_when_eaten());
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

//NOTE unused right now, kept for possible later use
//agent agent::nearest_agent(const game& g, agent& a, agent_type type){
//  double minD = pythagoras(1000000, 1000000);
//  agent near_agent(type);
//  for(const agent& ag: g.get_agents()){
//    if(ag.get_type() == type){
//      double distance = pythagoras(fabs(ag.get_x() - a.get_x()), fabs(ag.get_y() - a.get_y()));
//      if(distance < minD){
//        minD = distance;
//        near_agent = ag;
//      }
//    }
//  }
//  return near_agent;
//}

void agent::move(double x, double y)
{
  m_x += x;
  m_y += y;
}

void agent::move(game &g){ //!OCLINT too complex indeed
  // Plants don't move to their food
  if (is_plant(m_type)) {
    return;
  }

  //An exhausted agent loses health
  if (m_stamina <= 0.0) {
    const double change_in_health = 0.2 * (m_stamina - 1.0);
    assert(change_in_health <= 0.0);
    m_health += change_in_health;
  }

  //Dead agents stay still
  if (m_health <= 0.0) return;

  //Corpses stay still
  if (m_type == agent_type::corpse) return;

  //Move randomly a bit
  double temp_x = 0.1 * (-1 + (std::rand() % 3));
  double temp_y = 0.1 * (-1 + (std::rand() % 3));
  sf::Vector2f center_temp = get_agent_center(*this);
  std::vector<tile> t_temp = get_current_tile(g, center_temp.x + temp_x, center_temp.y + temp_y);
  if((is_on_tile(g, center_temp.x + temp_x, center_temp.y + temp_y)
     && t_temp[0].get_type() != tile_type::water)
     || (!will_drown(m_type, 12.34) && is_on_tile(g, center_temp.x + temp_x, center_temp.y + temp_y))
     || m_type == agent_type::bird){
    m_x += temp_x;
    m_y += temp_y;
  }

//  As long as we don't have a random seed, this can't be used here
//  m_x += 0.1 * (-1 + random_double(0, 3));
//  m_y += 0.1 * (-1 + random_double(0, 3));

//  This doesn't work properly
//
//  unsigned int rand = static_cast<unsigned int>(random_int(0, count_n_agents(g) - 1,
//                                                std::rand()));
//  std::cout << rand << std::endl;
//  agent a = g.get_agents()[rand];
//  if(std::find(m_prey.begin(), m_prey.end(), a.get_type()) != m_prey.end()){
//    double distance = pythagoras(fabs(m_x - a.get_x()), fabs(m_y - a.get_y()));
//    const double vector_length = std::exp(-distance/1000);
//    m_dx_motivation += -(0.01 * (m_x - a.get_x())) / vector_length;
//    m_dy_motivation += -(0.01 * (m_y - a.get_y())) / vector_length;
//    //std::cout << vector_length << " " << rand << " " << count_n_agents(g) << std::endl;
//    //std::cout << m_dx_motivation << " + " << m_dy_motivation << " " << rand << std::endl;
//    m_x += std::max(-0.35, std::min(m_dx_motivation, 0.35));
//    m_y += std::max(-0.35, std::min(m_dy_motivation, 0.35));
//  }
  if (!destination.empty())
  {
    double x = -(0.005 * (m_x - destination[0].get_x()));
    x = std::max(-0.05, std::min(x, 0.05));
    double y = -(0.005 * (m_y - destination[0].get_y()));
    y = std::max(-0.05, std::min(y, 0.05));
    sf::Vector2f center = get_agent_center(*this);
    std::vector<tile> t = get_current_tile(g, center.x + x*2, center.y + y*2);
    if((is_on_tile(g, center.x + x*2, center.y + y*2)
       && t[0].get_type() != tile_type::water)
       || m_type == agent_type::bird){
      m_x += x;
      m_y += y;
    }
  }
}


void agent::find_destination(game &g){
  std::vector<agent> v;
  for(const agent& a : g.get_agents())
  {
    if(std::find(m_prey.begin(), m_prey.end(), a.get_type()) == m_prey.end()) continue;
    if (v.empty())
    {
      v.push_back(a);
    }
    else
    {
      const double distance_v = pythagoras(fabs(m_x - v[0].get_x()), fabs(m_y - v[0].get_y()));
      const double distance_a = pythagoras(fabs(m_x - a.get_x()), fabs(m_y - a.get_y()));
      if (distance_a <distance_v)
      {
        v[0] = a;
      }
    }
  }
  destination = v;
}

void agent::attract_to_agent(game &g, agent_type type){
  std::vector<agent> v;
  for(const agent& a : g.get_agents())
  {
    if(a.get_type() != type) continue;
    if (v.empty())
    {
      v.push_back(a);
    }
    else
    {
      const double distance_v = pythagoras(abs(m_x - v[0].get_x()), abs(m_y - v[0].get_y()));
      const double distance_a = pythagoras(abs(m_x - a.get_x()), abs(m_y - a.get_y()));
      if (distance_a <distance_v)
      {
        v[0] = a;
      }
    }
  }
  if (!v.empty())
  {
    double x = -(0.01 * (m_x - v[0].get_x()));
    x = std::max(-0.05, std::min(x, 0.05));
    m_x += x;
    double y = -(0.01 * (m_y - v[0].get_y()));
    y = std::max(-0.05, std::min(y, 0.05));
    m_y += y;
    return;
  }
}

std::vector<agent> agent::process_events(game& g) { //!OCLINT NPath complexity too high
  //Do not change game::m_agents in this function!
  //Measure at start of function, will check at end as well
  const int n_agents_before = static_cast<int>(g.get_agents().size());

  std::vector <agent> new_agents;

  //Sessile and aquatic species die instantly when on void
  if(m_type != agent_type::bird && !is_on_tile(g, *this) && m_type != agent_type::corpse)
  {
    m_health = 0.0;
    return new_agents;
  }

  if(m_type == agent_type::corpse && corpse_ticks == -1){
    corpse_ticks = g.get_n_ticks();
  }

  //Agents always lose stamina
  m_stamina -= 0.0175;

  if(g.get_n_ticks() % 100 == 0) find_destination(g);

  move(g);

  if(m_type == agent_type::spider) attract_to_agent(g, agent_type::venus_fly_trap);


  if ((m_type == agent_type::grass || m_type == agent_type::tree
      || m_type == agent_type::cow) && g.allow_damage())
  {
    const auto kids = reproduce_agents(g, m_type);
    std::copy(std::begin(kids), std::end(kids), std::back_inserter(new_agents));
  }

  //Plants damage each other when nearby
  if (is_plant(m_type))
    damage_own_type(g, m_type);

  // Zero or one tile that the agent is on
  const std::vector<tile> tiles = get_on_tile(g, *this);
  if (tiles.size() ==1 && tiles[0].get_type() == tile_type::water)
  {
    // A water tile, on which agents can drown
    if (will_drown(m_type, tiles[0].get_depth())
      && !get_on_tile_type(g, *this).empty()
      && get_on_tile_type(g, *this).front() == tile_type::water)
    {
      m_stamina -= 0.2;
    }
  }


  ///Eating others
  eat(g);

  if(m_type == agent_type::fish || m_type == agent_type::whale){
    for(tile& t: g.get_tiles()){
      if(is_on_specific_tile(*this, t) && t.get_type() != tile_type::water){
        m_health -= 0.01;
      }
    }
  }

  //Do not change game::m_agents in this function!
  //Measure at end of function
  const int n_agents_after = static_cast<int>(g.get_agents().size());
  assert(n_agents_before == n_agents_after);

  return new_agents;
}

std::vector <agent> agent::reproduce_agents(game& g, agent_type type) { //!OCLINT indeed to complex, but get this merged first :-)

  std::vector <agent> new_agents;

  if(is_plant(type)){
    const double rand = random_double(10, 26) / 1000.0; // 20 extra for the grass self-damage
    // Grow
    m_health += rand;
  }

  if (m_health > get_agent_reproduction_health(type))
  {
    //Random fractions, from 0.0 to 1.0
    const double f_parent{random_double(0, 1)};
    const double f_kid{random_double(0, 1)};
    assert(f_parent >= 0.0 && f_parent < 1.0);
    assert(f_kid >= 0.0 && f_kid < 1.0);

    //Converted to proportions
    //Parent agent will get 0.4-0.6 of health
    //Kid    agent will get 0.1-0.3 of health
    const double p_parent{0.4 + (0.2 * f_parent)};
    const double p_kid{0.1 + (0.2 * f_parent)};
    assert(p_parent >= 0.4 && p_parent < 0.6);
    assert(p_kid >= 0.1 && p_kid < 0.3);

    //Convert to new healths
    const double health_parent{p_parent * m_health};
    const double health_kid{p_kid * m_health};

    //Kids grow at new spot
    const double max_distance{64.0};
    double f_x = 0;
    double f_y = 0;
    double new_x = 0;
    double new_y = 0;

    agent new_agent(type, new_x, new_y, health_kid, 0, can_eat(type));
    std::vector<tile> t;
//    bool water = get_on_tile_type(g, new_agent).size() > 0 &&
//                 get_on_tile_type(g, new_agent).at(0) == tile_type::water;
    while (t.empty()
           || !is_on_tile(g, new_agent)
//           || !water
           || !is_on_specific_tile(new_agent.get_x() - 6, new_agent.get_y() - 6, t.front())
           || !is_on_specific_tile(new_agent.get_x() + 18, new_agent.get_y() + 18, t.front())
    )
    {
//    As long as we don't have a random seed, this can't be used here
//    f_x = random_double(0, 1);
//    f_y = random_double(0, 1);
      f_x = static_cast<double>(std::rand()) / (1.0 + static_cast<double>(RAND_MAX));
      f_y = static_cast<double>(std::rand()) / (1.0 + static_cast<double>(RAND_MAX));
      assert(f_x >= 0.0 && f_x < 1.0);
      assert(f_y >= 0.0 && f_y < 1.0);
      new_x = m_x + (((f_x * 2.0) - 1.0) * max_distance);
      new_y = m_y + (((f_y * 2.0) - 1.0) * max_distance);
      new_agent.set_x(new_x);
      new_agent.set_y(new_y);
      if(is_on_tile(g, new_agent)){
        t = get_current_tile(g, new_agent);
//        water = get_on_tile_type(g, new_agent).size() > 0 &&
//                get_on_tile_type(g, new_agent).at(0) == tile_type::water;
      }
    }
    // g.add_agents( { new_agent } );
    m_health = health_parent;

    new_agents.push_back(new_agent);
//    water = get_on_tile_type(g, new_agent).size() > 0 &&
//            get_on_tile_type(g, new_agent).at(0) == tile_type::water;
  }

  return new_agents;
}

void agent::damage_own_type(game &g, agent_type type)
{
  const double MAX_DISTANCE = 30; // The max range to deal damage to an object
  const double MAX_DAMAGE = 0.18; // The max damage to deal per frame per agent

//  std::vector <agent> all_agents{ g.get_agents() };

//  for (agent& current_agent : all_agents)
//  {
//    if (current_agent == *this)
//        continue;

//    if (current_agent.get_type() == type)
//    {
//        double distance = pythagoras(fabs(current_agent.get_x() - m_x), fabs(current_agent.get_y() - m_y));
//        if (!(distance <= MAX_DISTANCE))
//          continue;

//        double rate = 1-distance / MAX_DISTANCE;
//        double damage = MAX_DAMAGE * rate;
//        double relative_damage = damage / (all_agents.size() - 1);
//        m_health -= relative_damage;
//    }
//  }
  agent a = g.get_agents()[static_cast<unsigned>(random_int(0, static_cast<int>(g.get_agents().size() - 1)))];
  if (a == *this) return;
  if (a.get_type() == type)
  {
    double distance = pythagoras(fabs(a.get_x() - m_x), fabs(a.get_y() - m_y));
    if (!(distance <= MAX_DISTANCE))
       return;

    double rate = 1-distance / MAX_DISTANCE;
    double damage = MAX_DAMAGE * rate;
    double relative_damage = damage / (g.get_agents().size() - 1);
    m_health -= relative_damage;
  }
}

std::vector<agent> create_default_agents() noexcept //!OCLINT indeed too long
{
  std::vector<agent> agents;
  {
    agent a1(agent_type::chameleon, 0, 0, 1, 0, can_eat(agent_type::chameleon));
    move_agent_to_tile(a1, -3, 0);
    agents.push_back(a1);
  }
  {
    agent a1(agent_type::cow, 0, 0, 1, 0, can_eat(agent_type::cow));
    move_agent_to_tile(a1, 0, 0);
    agents.push_back(a1);
    agent a2(agent_type::cow, 40, 70, 1, 0, can_eat(agent_type::cow));
    move_agent_to_tile(a2, 0, 0);
    agents.push_back(a2);
    agent a3(agent_type::grass, 70, 40, 50 + random_double(0, 50)
             , 0, can_eat(agent_type::grass));
    move_agent_to_tile(a3, 0, 0);
    agents.push_back(a3);
    agent a4(agent_type::sunflower, 42, 112, 1, 0, can_eat(agent_type::sunflower));
    agent a5(agent_type::foxgloves, 60, 70, 1, 0, can_eat(agent_type::foxgloves));
    move_agent_to_tile(a4, 0, 0);
    agents.push_back(a4);
    move_agent_to_tile(a5, 0, 0);
    agents.push_back(a5);
  }
  {
    agent a1(agent_type::cow, 0, 0, 1, 0, can_eat(agent_type::cow));
    move_agent_to_tile(a1, 1, 0);
    agents.push_back(a1);
    agent a2(agent_type::cow, 90, 30, 1, 0, can_eat(agent_type::cow));
    move_agent_to_tile(a2, 1, 0);
    agents.push_back(a2);
    agent a3(agent_type::cow, 30, 90, 1, 0, can_eat(agent_type::cow));
    move_agent_to_tile(a3, 1, 0);
    agents.push_back(a3);
    agent a4(agent_type::worm, 50, 130, 1, 0, can_eat(agent_type::worm));
    move_agent_to_tile(a4, 1, 0);
    agents.push_back(a4);
  }
  {
    agent a1(agent_type::crocodile, 30, 160, 1, 0, can_eat(agent_type::crocodile));
    move_agent_to_tile(a1, 0, 2);
    agents.push_back(a1);
    agent a2(agent_type::snake, 50, 15, 1, 0, can_eat(agent_type::snake));
    move_agent_to_tile(a2, 0, 2);
    agents.push_back(a2);
    agent a3(agent_type::venus_fly_trap, 30, 160, 1000, 0,
             can_eat(agent_type::venus_fly_trap));
    move_agent_to_tile(a3, 0, 2);
    agents.push_back(a3);
    agent a4(agent_type::cactus, 10, 15, 100, 0, can_eat(agent_type::cactus));
    move_agent_to_tile(a4, 0, 2);
    agents.push_back(a4);
    agent a5(agent_type::cactus, 30, 120, 100, 0, can_eat(agent_type::cactus));
    move_agent_to_tile(a5, 0, 2);
    agents.push_back(a5);
  }
  {
    agent a1(agent_type::crocodile, 0, 0, 1, 0, can_eat(agent_type::crocodile));
    move_agent_to_tile(a1, 2, 1);
    agents.push_back(a1);
    agent a2(agent_type::grass, 0, 0, 50 + random_double(0, 50),
             0, can_eat(agent_type::grass));
    move_agent_to_tile(a2, 2, 1);
    agents.push_back(a2);
  }
  {
    agent a1(agent_type::fish, 0, 0, 1, 0, can_eat(agent_type::fish));
    move_agent_to_tile(a1, 3, 2);
    agents.push_back(a1);
    agent a2(agent_type::fish, 10, 10, 1, 0, can_eat(agent_type::fish));
    move_agent_to_tile(a2, 3, 2);
    agents.push_back(a2);
    agent a3(agent_type::octopus, 50, 70, 1, 0, can_eat(agent_type::octopus));
    move_agent_to_tile(a3, 3, 2);
    agents.push_back(a3);
  }
  {
    agent a1(agent_type::whale, 0, 0, 1, 0, can_eat(agent_type::whale));
    move_agent_to_tile(a1, 3, 2);
    agents.push_back(a1);
  }
  {
    agent a1(agent_type::whale, 0, 0, 1, 0, can_eat(agent_type::whale));
    move_agent_to_tile(a1, 3, 2);
    agents.push_back(a1);
  }
  {
    agent a1(agent_type::fish, 0, 0, 1, 0, can_eat(agent_type::fish));
    move_agent_to_tile(a1, 4, 2);
    agents.push_back(a1);
    agent a2(agent_type::fish, 10, 10, 1, 0, can_eat(agent_type::fish));
    move_agent_to_tile(a2, 4, 2);
    agents.push_back(a2);
  }
  {
    agent a1(agent_type::grass, 0, 0, 50 + random_double(0, 50),
             0, can_eat(agent_type::grass));
    move_agent_to_tile(a1, 1, -1);
    agents.push_back(a1);
    agent a2(agent_type::giraffe, 10, 20, 1, 0, can_eat(agent_type::giraffe));
    move_agent_to_tile(a2, 1, -1);
    agents.push_back(a2);
    //agent a3(agent_type::lion, 120, 20);
    //move_agent_to_tile(a3, 1, -1);
    //agents.push_back(a3);
    agent a4(agent_type::crocodile, 180, 20, 1, 0, can_eat(agent_type::crocodile));
    move_agent_to_tile(a4, 1, -1);
    agents.push_back(a4);
  }
  {
    agent a1(agent_type::tree, 10, 20, 1, 0, can_eat(agent_type::tree));
    move_agent_to_tile(a1, 4, -1);
    agents.push_back(a1);
    agent a2(agent_type::tree, 40, 10, 1, 0, can_eat(agent_type::tree));
    move_agent_to_tile(a2, 4, -1);
    agents.push_back(a2);
    agent a3(agent_type::tree, 50, 35, 1, 0, can_eat(agent_type::tree));
    move_agent_to_tile(a3, 4, -1);
    agents.push_back(a3);
    agent a4(agent_type::tree, 60, 40, 1, 0, can_eat(agent_type::tree));
    move_agent_to_tile(a4, 4, -1);
    agents.push_back(a4);
    agent a5(agent_type::tree, 35, 65, 1, 0, can_eat(agent_type::tree));
    move_agent_to_tile(a5, 4, -1);
    agents.push_back(a5);
    agent a6(agent_type::tree, 0, 0, 1, 0, can_eat(agent_type::tree));
    move_agent_to_tile(a6, 4, -1);
    agents.push_back(a6);
    agent a7(agent_type::spider, 40, 40, 1, 0, can_eat(agent_type::spider));
    move_agent_to_tile(a7, 4, -1);
    agents.push_back(a7);
    agent a8(agent_type::bird, 75, 150, 1, 0, can_eat(agent_type::bird));
    move_agent_to_tile(a8, 4, -1);
    agents.push_back(a8);
  }
  {
    agent a1(agent_type::tree, 90, 170, 1, 0, can_eat(agent_type::tree));
    move_agent_to_tile(a1, 0, -2);
    agents.push_back(a1);
    agent a2(agent_type::squirrel, 90, 150, 1, 0, can_eat(agent_type::squirrel));
    move_agent_to_tile(a2, 0, -2);
    agents.push_back(a2);
  }
  {
    agent a1(agent_type::polar_bear, 4, 112, 1, 0, can_eat(agent_type::polar_bear));
    move_agent_to_tile(a1, 5, 0);
    agents.push_back(a1);
    agent a2(agent_type::polar_bear, 4, 112, 1, 0, can_eat(agent_type::polar_bear));
    move_agent_to_tile(a2, 4, 0);
    agents.push_back(a2);
  }
  {
    agent a1(agent_type::goat, 190, 90, 1, 0, can_eat(agent_type::goat));
    move_agent_to_tile(a1, 1, 2);
    agents.push_back(a1);
    agent a2(agent_type::goat, 50, 80, 1, 0, can_eat(agent_type::goat));
    move_agent_to_tile(a2, 1, 2);
    agents.push_back(a2);
  }
  return agents;
}

void move_agent_to_tile(agent &a, double tile_x, double tile_y) {
  a.set_x(a.get_x()+(tile_x*112));
  a.set_y(a.get_y()+(tile_y*112));
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

bool will_drown(agent_type a, const int depth) { //!OCLINT can't be simpler
  const auto range = get_depth_range(a);
  switch (a) {
    case agent_type::plankton:
      return depth > range.x && depth <= range.y;
  case agent_type::worm:
    return false;
    case agent_type::bird:
      return false;
    case agent_type::cow:
      return true;
    case agent_type::giraffe:
      return true;
    case agent_type::crocodile:
      return depth > range.x && depth <= range.y;
    case agent_type::chameleon:
      return true;
    case agent_type::fish:
      return depth > range.x && depth <= range.y;
    case agent_type::whale:
      return false;
    case agent_type::goat:
      return true;
    case agent_type::spider:
      return true;
    case agent_type::octopus:
      return depth > range.x && depth <= range.y;
    case agent_type::polar_bear:
      return false;
    case agent_type::snake:
      return true;
    default:
      return true;
  }
}

bool is_auqatic(agent_type a){
    return a == agent_type::fish ||
           a == agent_type::whale ||
           a == agent_type::octopus ||
           a == agent_type::plankton ||
           a == agent_type::crocodile;
}

int get_min_depth(agent_type a){
    switch (a) {
        case agent_type::fish:
            return 0;
        case agent_type::whale:
            return 50;
        case agent_type::octopus:
            return 25;
        case agent_type::plankton:
            return 25;
        case agent_type::crocodile:
            return 0;
        default:
            return 0;
    }
}

int get_max_depth(agent_type a){
    switch (a) {
        case agent_type::fish:
            return 50;
        case agent_type::whale:
            return 100;
        case agent_type::octopus:
            return 75;
        case agent_type::plankton:
            return 75;
        case agent_type::crocodile:
            return 25;
        default:
            return 0;
    }
}

sf::Vector2i get_depth_range(agent_type a){
    return sf::Vector2i(get_min_depth(a), get_max_depth(a));
}


void test_agent() //!OCLINT testing functions may be long
{
  #define FIX_ISSUE_447
  #ifdef FIX_ISSUE_447
  //Cacti damage nearby cacti
  {
    // Make two plants next to each other.
    game g({tile(0, 0, 3, 3, 10, tile_type::grassland)},
           {agent(agent_type::cactus, 10, 10, 10),
            agent(agent_type::cactus, 10, 10, 10)});
    sound_type st { sound_type::none };

    // Check their initial health.
    const double prev_health1 = g.get_agents()[0].get_health();
    const double prev_health2 = g.get_agents()[1].get_health();

    // Damage time
    for(int i = 0; i != 100; ++i){
      g.process_events(st);
    }

    // Check their health after doing damage
    const double after_health1 = g.get_agents()[0].get_health();
    const double after_health2 = g.get_agents()[1].get_health();

    // Plants should have damaged each other
    assert(after_health1 < prev_health1);
    assert(after_health2 < prev_health2);
  }
  //Foxgloves damage nearby Foxgloves
  {
    // Make two plants next to each other.
    game g({tile(0, 0, 3, 3, 10, tile_type::grassland)},
           {agent(agent_type::foxgloves, 10, 10, 10),
            agent(agent_type::foxgloves, 10, 10, 10)});
    sound_type st { sound_type::none };

    // Check their initial health.
    const double prev_health1 = g.get_agents()[0].get_health();
    const double prev_health2 = g.get_agents()[1].get_health();

    // Damage time.
    for(int i = 0; i != 100; ++i){
      g.process_events(st);
    }

    // Check their health after doing damage
    const double after_health1 = g.get_agents()[0].get_health();
    const double after_health2 = g.get_agents()[1].get_health();

    // Plants should have damaged each other
    assert(after_health1 < prev_health1);
    assert(after_health2 < prev_health2);
  }
  //Plankton damage nearby Plankton
  {
    // Make two plants next to each other.
    game g({tile(0, 0, 3, 3, 10, tile_type::water)},
           {agent(agent_type::plankton, 10, 10, 10),
            agent(agent_type::plankton, 10, 10, 10)});
    sound_type st { sound_type::none };

    // Check their initial health.
    const double prev_health1 = g.get_agents()[0].get_health();
    const double prev_health2 = g.get_agents()[1].get_health();

    // Damage time.
    for(int i = 0; i != 100; ++i){
      g.process_events(st);
    }

    // Check their health after doing damage
    const double after_health1 = g.get_agents()[0].get_health();
    const double after_health2 = g.get_agents()[1].get_health();

    // Plants should have damaged each other
    assert(after_health1 < prev_health1);
    assert(after_health2 < prev_health2);
  }
  //Sunflowers damage nearby Sunflowers
  {
    // Make two plants next to each other.
    game g({tile(0, 0, 3, 3, 10, tile_type::grassland)},
           {agent(agent_type::sunflower, 10, 10, 10),
            agent(agent_type::sunflower, 10, 10, 10)});
    sound_type st { sound_type::none };

    // Check their initial health.
    const double prev_health1 = g.get_agents()[0].get_health();
    const double prev_health2 = g.get_agents()[1].get_health();

    // Damage time.
    for(int i = 0; i != 100; ++i){
      g.process_events(st);
    }

    // Check their health after doing damage
    const double after_health1 = g.get_agents()[0].get_health();
    const double after_health2 = g.get_agents()[1].get_health();

    // Plants should have damaged each other
    assert(after_health1 < prev_health1);
    assert(after_health2 < prev_health2);
  }
  //Venus fly traps damage nearby Venus fly traps
  {
    // Make two plants next to each other.
    game g({tile(0, 0, 3, 3, 10, tile_type::grassland)},
           {agent(agent_type::venus_fly_trap, 10, 10, 10),
            agent(agent_type::venus_fly_trap, 10, 10, 10)});
    sound_type st { sound_type::none };

    // Check their initial health.
    const double prev_health1 = g.get_agents()[0].get_health();
    const double prev_health2 = g.get_agents()[1].get_health();

    // Damage time.
    for(int i = 0; i != 100; ++i){
      g.process_events(st);
    }

    // Check their health after doing damage
    const double after_health1 = g.get_agents()[0].get_health();
    const double after_health2 = g.get_agents()[1].get_health();

    // Plants should have damaged each other
    assert(after_health1 < prev_health1);
    assert(after_health2 < prev_health2);
  }
  #endif // FIX_ISSUE_447
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
  // Check if species are classified as plants
  {
    assert(!is_plant(agent_type::bird));
    assert(!is_plant(agent_type::cow));
    assert(!is_plant(agent_type::chameleon));
    assert(!is_plant(agent_type::crocodile));
    assert(!is_plant(agent_type::fish));
    assert(!is_plant(agent_type::giraffe));
    assert(!is_plant(agent_type::goat));
    assert( is_plant(agent_type::grass));
    assert(!is_plant(agent_type::lion));
    assert(!is_plant(agent_type::octopus));
    assert( is_plant(agent_type::plankton));
    assert(!is_plant(agent_type::snake));
    assert(!is_plant(agent_type::polar_bear));
    assert(!is_plant(agent_type::spider));
    assert(!is_plant(agent_type::squirrel));
    assert( is_plant(agent_type::cactus));
    assert( is_plant(agent_type::tree));
    assert( is_plant(agent_type::venus_fly_trap));
    assert(!is_plant(agent_type::whale));
    assert(!is_plant(agent_type::worm));
  }
  // A cow moves
  {
    game g;
    const double x{12.345};
    const double y{56.789};
    agent a(agent_type::cow, x, y);
    assert(is_on_tile(g, a));
    for (int i = 0; i < 50; i++) a.move(g);
    assert(a.get_x() != x || a.get_y() != y);
  }
  // A crocodile moves
  {
    game dummy_game; //Unused
    const double x{12.345};
    const double y{56.789};
    agent a(agent_type::crocodile, x, y);
    for (int i = 0; i != 50; ++i) a.move(dummy_game); //To make surer x or y is changed
    assert(a.get_x() != x || a.get_y() != y);
  }
  // A fish moves
  {
    game g;
    const double x{12.345};
    const double y{56.789};
    agent a(agent_type::fish, x, y);
    assert(is_on_tile(g, a));
    for (int i = 0; i < 50; i++) a.move(g);
    assert(a.get_x() != x || a.get_y() != y);
  }
  // A bird moves
  {
    game g;
    const double x{12.345};
    const double y{56.789};
    agent a(agent_type::bird, x, y);
    assert(is_on_tile(g, a));
    for(int i = 0; i < 10; i++) a.move(g);
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
  // Venus Fly Trap does not move
  {
    game g;
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::venus_fly_trap, x, y);
    assert(is_on_tile(g, a));
    a.move(g);
    assert(a.get_x() == x && a.get_y() == y);
  }
  // Agents have health
  {
    const agent a(agent_type::cow, 0, 0, 10);
    assert(a.get_health() > 0.0);
  }
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
  // Test can_eat
  {
    for (agent_type a : collect_all_agent_types()) {
      can_eat(a);
    }
    {   // Cow can eat grass
        std::vector<agent_type> vec = can_eat(agent_type::cow);
        assert(vec.at(0) == agent_type::grass);
    }
    {   // Crocodile can eat cows
        std::vector<agent_type> vec = can_eat(agent_type::crocodile);
        assert(vec.at(0) == agent_type::cow);
    }
  }
  //Agent can pass out of exhaustion
  {
    game g(create_test_default_tiles(), { agent(agent_type::cow) } );
    sound_type st { sound_type::none };
    assert(!g.get_agents().empty());
    const auto stamina_before = g.get_agents()[0].get_stamina();
    // Exhaust one turn
    g.process_events(st);
    const auto stamina_after = g.get_agents()[0].get_stamina();
    assert(stamina_after < stamina_before);
  }
  //A cow must starve if alone
  {
    game g({ tile(0 * 112, 0 * 112, 0, 2, 2) }, { agent(agent_type::cow) } );
    sound_type st { sound_type::none };
    assert(!g.get_agents().empty());
    const auto health_before = g.get_agents()[0].get_health();
    //Exhaust cow
    while (g.get_agents()[0].get_stamina() > 0.0)
    {
      g.process_events(st);
    }
    // Starve one turn
    g.process_events(st);
    const auto health_after = g.get_agents()[0].get_health();
    assert(health_after < health_before);
  }
  //An agent must be removed if health is below zero
  {
    game g({tile(0, 0, 0, 90, 0, tile_type::grassland)}, { agent(agent_type::cow, 50, 50) } );
    sound_type st { sound_type::none };
    g.set_allow_spawning(false);
    assert(!g.get_agents().empty());
    // Wait until cow starves
    while (g.get_agents()[0].get_type() != agent_type::corpse)
    {
      g.process_events(st);
    }
  }
  //Grass grows
  {
    game g(create_test_default_tiles(), { agent(agent_type::grass) } );
    sound_type st { sound_type::none };
    assert(!g.get_agents().empty());
    const auto health_before = g.get_agents()[0].get_health();
    // Grow one turn
    g.process_events(st);
    const auto health_after = g.get_agents()[0].get_health();
    assert(health_after > health_before);
  }
  //Trees grow
  {
    game g(create_test_default_tiles(), { agent(agent_type::tree) } );
    sound_type st { sound_type::none };
    assert(!g.get_agents().empty());
    const auto health_before = g.get_agents()[0].get_health();
    // Grow one turn
    g.process_events(st);
    const auto health_after = g.get_agents()[0].get_health();
    assert(health_after > health_before);
  }
  //Sessile agents that move on nothing get zero health
  {
    const std::vector<tile> no_tiles;
    game g(no_tiles, { agent(agent_type::crocodile, -100, -100, 100)});
    sound_type st { sound_type::none };
    assert(g.get_agents()[0].get_health() > 0.0); //!OCLINT accepted idiom
    g.process_events(st);
    assert(g.get_agents()[0].get_type() == agent_type::corpse); //!OCLINT accepted idiom
  }
  //#define FIX_ISSUE_300
  #ifdef FIX_ISSUE_300
  //Grass creates new grasses
  {
    game g(create_default_tiles(), { agent(agent_type::grass) } );
    sound_type st { sound_type::none };
    assert(g.get_agents().size() == 1);
    while (g.get_agents().size() == 1) //Wait until grass procreates
    {
      g.process_events(st);
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
      create_test_default_tiles(),
      {
        agent(agent_type::grass, 0.0, 0.0, grass_health, 0, can_eat(agent_type::grass)),
        agent(agent_type::cow  , 0.0, 0.0, 10.0, 0, can_eat(agent_type::cow))
      }
    );
    sound_type st { sound_type::none };
    const double grass_health_before = g.get_agents()[0].get_health();
    const double cow_stamina_before = g.get_agents()[1].get_stamina();
    g.process_events(st);
    const double grass_health_after = g.get_agents()[0].get_health();
    const double cow_stamina_after = g.get_agents()[1].get_stamina();
    //Grass is eaten ...
    assert(grass_health_after < grass_health_before);
    //Cow is fed ...
    assert(cow_stamina_after > cow_stamina_before);
  }
  //Crocodiles eat cows
  {
    const double cow_health{5.0};
    game g(
      create_test_default_tiles(),
      {
        agent(agent_type::cow, 0.0, 0.0, cow_health, 0, can_eat(agent_type::cow)),
        agent(agent_type::crocodile  , 0.0, 0.0, 10.0, 0, can_eat(agent_type::crocodile))
      }
    );
    sound_type st { sound_type::none };
    assert(g.get_agents()[0].get_health() == cow_health);
    double crocodile_stamina = g.get_agents()[1].get_stamina();
    g.process_events(st);
    //Grass is eaten ...
    assert(g.get_agents()[0].get_health() < cow_health);
    //Cow is fed ...
    assert(g.get_agents()[1].get_stamina() > crocodile_stamina);
  }
  //Fish die when on land
  {
    game g({ tile(0, 0, 0, 90, 0, tile_type::grassland) }, { agent(agent_type::fish) } );
    sound_type st { sound_type::none };
    assert(!g.get_agents().empty());
    //Choke fish
    while (g.get_agents()[0].get_type() != agent_type::corpse)
    {
      g.process_events(st);
    }
  }
  //octopus die when on land
  {
    game g({ tile(0, 0, 0, 90, 0, tile_type::grassland) }, { agent(agent_type::octopus) } );
    sound_type st { sound_type::none };
    assert(!g.get_agents().empty());
    //Choke octopus
    while (g.get_agents()[0].get_type() != agent_type::corpse)
    {
      g.process_events(st);
    }
  }
  // Agents drown in very very deep water
  {
    game g({ tile(0, 0, 0, 90, 100, tile_type::water)},
           {agent(agent_type::cow, 10, 10),
            agent(agent_type::fish, 10, 10)});
    sound_type st { sound_type::none };
    double cow_before = g.get_agents()[0].get_stamina();
    double fish_before = g.get_agents()[1].get_stamina();
    g.process_events(st);
    double delta_cow = cow_before - g.get_agents()[0].get_stamina();
    double delta_fish = fish_before - g.get_agents()[1].get_stamina();
    assert(delta_fish < delta_cow);
  }
  //grass grows gradually
  {
    game g({tile(0, 0, 0, 0, 10, tile_type::grassland)},
           {agent(agent_type::grass, 10, 10),
            agent(agent_type::grass, 90, 202)});
    sound_type st { sound_type::none };
    const auto prev_grass_health1 = g.get_agents()[0].get_health();
    const auto prev_grass_health2 = g.get_agents()[1].get_health();
    for(int i = 0; i < 10; i++){
      g.process_events(st);
    }
    const auto after_grass_health1 = g.get_agents()[0].get_health();
    const auto after_grass_health2 = g.get_agents()[1].get_health();
    const auto grass1_delta = after_grass_health1 - prev_grass_health1;
    const auto grass2_delta = after_grass_health2 - prev_grass_health2;
    assert(grass1_delta != grass2_delta);
  }
  //grass has different health when its duplicated
  {
    game g({tile(0, 0, 0, 90, 10, tile_type::grassland)},
           {agent(agent_type::grass, 10, 10, 10000)});
    sound_type st { sound_type::none };
    const auto prev_health = g.get_agents()[0].get_health();

    g.process_events(st);
    assert(g.get_agents().size() == 2);
    const auto after_health = g.get_agents()[0].get_health();
    const auto second_grass_health = g.get_agents()[1].get_health();
    assert(prev_health != after_health);
    assert(after_health != second_grass_health);
  }
  //get depth test
  {
    assert(get_depth_range(agent_type::fish) == sf::Vector2i(0, 50));
  }
  //a cow walks to grass when its close
  {
    game g(create_test_default_tiles(),
           {agent(agent_type::cow, 0, 0, 100, 0, can_eat(agent_type::cow)),
            agent(agent_type::grass, 100, 100, 100, 0, can_eat(agent_type::grass))});
    sound_type st { sound_type::none };
    double cow_prev_posX = g.get_agents()[0].get_x();
    double cow_prev_posY = g.get_agents()[0].get_y();
    double distanceX = g.get_agents()[1].get_x() - g.get_agents()[0].get_x();
    double distanceY = g.get_agents()[1].get_y() - g.get_agents()[0].get_y();
    //move the cow 100 times
    for(int i = 0; i < 1000; i++){
      g.process_events(st);
    }
    double cow_aft_posX = g.get_agents()[0].get_x();
    double cow_aft_posY = g.get_agents()[0].get_y();
    double distance_afterX = g.get_agents()[1].get_x() - g.get_agents()[0].get_x();
    double distance_afterY = g.get_agents()[1].get_y() - g.get_agents()[0].get_y();
    assert(distanceX > distance_afterX);
    assert(distanceY > distance_afterY);
    assert(g.get_agents()[0].get_x() > 0);
    assert(g.get_agents()[0].get_y() > 0);
    assert(cow_prev_posX < cow_aft_posX);
    assert(cow_prev_posY < cow_aft_posY);
  }
  //a spider is attracted to venus_fly_trap
  {
    game g(create_test_default_tiles(),
           {agent(agent_type::spider, 0, 0),
            agent(agent_type::venus_fly_trap, 50, 50)});
    double spider_prev_posX = g.get_agents()[0].get_x();
    double spider_prev_posY = g.get_agents()[0].get_y();
    double distanceX = fabs(g.get_agents()[1].get_x() - g.get_agents()[0].get_x());
    double distanceY = fabs(g.get_agents()[1].get_y() - g.get_agents()[0].get_y());
    double distance = pythagoras(distanceX, distanceY);
    //move the spider 100 times
    for(int i = 0; i < 100; i++){
      //g.process_events() doesn't work?
      //g.process_events();
      g.get_agents()[0].attract_to_agent(g, agent_type::venus_fly_trap);
    }
    double spider_aft_posX = g.get_agents()[0].get_x();
    double spider_aft_posY = g.get_agents()[0].get_y();
    double distance_afterX = fabs(g.get_agents()[1].get_x() - spider_aft_posX);
    double distance_afterY = fabs(g.get_agents()[1].get_y() - spider_aft_posY);
    double distance_after = pythagoras(distance_afterX, distance_afterY);
    assert(distance > distance_after);
    assert(g.get_agents()[0].get_x() > 0);
    assert(g.get_agents()[0].get_y() > 0);
    assert(spider_prev_posX < spider_aft_posX);
    assert(spider_prev_posY < spider_aft_posY);
  }
  //Grass damages nearby grass
  {
    // Make two plants next to each other.
    game g({tile(0, 0, 3, 3, 10, tile_type::grassland)},
           {agent(agent_type::grass, 10, 10, 10),
            agent(agent_type::grass, 10, 10, 10)});
    sound_type st { sound_type::none };
    // Check their initial health.
    const double prev_health1 = g.get_agents()[0].get_health();
    const double prev_health2 = g.get_agents()[1].get_health();

    // Damage time
    for(int i = 0; i != 100; ++i){
      g.process_events(st);
    }

    // Check their health after doing damage
    const double after_health1 = g.get_agents()[0].get_health();
    const double after_health2 = g.get_agents()[1].get_health();

    // Plants should have damaged each other
    assert(after_health1 < prev_health1);
    assert(after_health2 < prev_health2);
  }
  #define FIX_ISSUE_447
  #ifdef FIX_ISSUE_447
  //Cacti damage nearby cacti
  {
    // Make two plants next to each other.
    game g({tile(0, 0, 0, 90, 10, tile_type::desert)},
           {agent(agent_type::cactus, 10, 10, 10),
            agent(agent_type::cactus, 10, 10, 10)});
    sound_type st { sound_type::none };

    // Check their initial health.
    const double prev_health1 = g.get_agents()[0].get_health();
    const double prev_health2 = g.get_agents()[1].get_health();

    // Damage time.
    for(int i = 0; i != 20; ++i){
      g.process_events(st);
    }

    // Check their health after doing damage
    const double after_health1 = g.get_agents()[0].get_health();
    const double after_health2 = g.get_agents()[1].get_health();

    // Plants should have damaged each other
    assert(after_health1 < prev_health1);
    assert(after_health2 < prev_health2);
  }
  #endif // FIX_ISSUE_447
  //Cows reproduce
  {
    game g({tile(0, 0, 0, 90, 10, tile_type::grassland)},
           {agent(agent_type::cow, 10, 10, 150)});
    sound_type st { sound_type::none };
    assert(g.get_agents().size() == 1);
    g.process_events(st);
    assert(g.get_agents().size() >= 2);
  }
  #define FIX_ISSUE_540
  #ifdef FIX_ISSUE_540
  {
    assert(get_agent_reproduction_health(agent_type::cactus) == 100.0);
    assert(get_agent_reproduction_health(agent_type::cow) == 100.0);
    assert(get_agent_reproduction_health(agent_type::grass) == 100.0);
    assert(get_agent_reproduction_health(agent_type::tree) == 500.0);
  }
  #endif // FIX_ISSUE_540
  {
    std::vector<agent> v1{};
    std::vector<agent> v2{};
    for (int i = 0; i < 5; i++) {
      agent a(agent_type::cow);
      v1.push_back(a);
    }
    for (int i = 0; i < 5; i++) {
      agent a(agent_type::cow);
      v2.push_back(a);
    }
    assert(v1 == v2);
  }
  {
    assert(is_auqatic(agent_type::whale));
  }
}
