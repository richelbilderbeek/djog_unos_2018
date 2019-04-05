#include "agent.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <set>
#include "agent_type.h"
#include "game.h"

using namespace sf;

agent::agent(
  const agent_type type,
  const double x,
  const double y,
  const double health,
  const double direction
) : m_type{type},
    m_x{x},
    m_y{y},
    m_direction{direction},
    m_health{health},
    m_stamina{100}
{
  //An agent of type none is used when a 'null' agent is needed
  assert(type != agent_type::none || type == agent_type::none);
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

double pythagoras(double x_length, double y_length)
{
  return sqrt(
    (x_length * x_length) + (y_length * y_length)
  );
}

std::vector<agent_type> can_eat(const agent_type type) {
  switch (type) {
    case agent_type::chameleon:
      return {agent_type::worm, agent_type::spider, agent_type::bird};
    case agent_type::crocodile:
      return {agent_type::cow, agent_type::giraffe};
    case agent_type::squirrel:
      return {agent_type::tree};
    case agent_type::snake:
      return {agent_type::squirrel};
    case agent_type::bird:
      return {agent_type::spider,
              agent_type::fish,
              agent_type::snake,
              agent_type::worm};
    case agent_type::cow:
      return {agent_type::grass};
    case agent_type::lion:
      return {agent_type::cow, agent_type::giraffe};
    case agent_type::giraffe:
      return {agent_type::tree};
    //case agent_type::venus_fly_trap:
    //  return {agent_type::spider };
    default:
      return {agent_type::none};
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

void agent::eat(const game& g) {
  std::vector<agent_type> food = can_eat(m_type);
  //Is agent_type a in food?
  for (agent a : g.get_agents()) {
    // NOTE not calculated from the center of the agent
    if (is_in_range(a.get_x(), a.get_y(), 25.0)
      && a.get_health() > 0.0
      && std::count(std::begin(food), std::end(food), a.get_type())
    )
    {
      m_stamina += 1.0;
    }
    else if (!is_plant(m_type))
    {
      m_stamina -= 0.05;
    }
    std::vector<agent_type> a_food = can_eat(a.get_type());
    // NOTE not calculated from the center of the agent
    if (is_in_range(a.get_x(), a.get_y(), 25.0)
      && m_health > 0.0
      && std::count(std::begin(a_food), std::end(a_food), m_type)
    )
    {
      m_stamina = 0.0;
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

agent agent::nearest_agent(game& g, agent& a, agent_type type){
  double minD = pythagoras(1000000, 1000000);
  agent near_agent(type);
  for(agent& ag: g.get_agents()){
    if(ag.get_type() == type){
      double distance = pythagoras(fabs(ag.get_x() - a.get_x()), fabs(ag.get_y() - a.get_y()));
      if(distance < minD){
        minD = distance;
        near_agent = ag;
      }
    }
  }
  return near_agent;
}

void agent::move() //!OCLINT NPath complexity too high
{
  //Dead agents stay still
  if (m_health <= 0.0) return;
  if (m_stamina <= 0.0) {
    m_health += (m_stamina - 1) * 0.2;
  }
  if (!is_plant(m_type) && m_type != agent_type::corpse) {
    m_x += 0.1 * (-1 + (std::rand() % 3));
    m_y += 0.1 * (-1 + (std::rand() % 3));
  }
}

void agent::move(double x, double y)
{
  m_x += x;
  m_y += y;
}

void agent::move_to_food(game &g){
  // Plants don't move to their food
  if (is_plant(m_type)) {
    return;
  }
  agent nearest_f(agent_type::none, INFINITY, INFINITY);
  double f_distance = pythagoras(fabs(m_x - nearest_f.get_x()), fabs(m_y - nearest_f.get_y()));
  double distance;
  for(agent a : g.get_agents()){
    for(int i = static_cast<int>(can_eat(m_type).size() - 1); i > -1; i--){
      if(a.get_type() == can_eat(m_type)[i]){
        distance = pythagoras(fabs(m_x - a.get_x()), fabs(m_y - a.get_y()));
        if(a == nearest_agent(g, *this, can_eat(m_type)[i])
           && distance < f_distance && distance < 200){
          nearest_f = a;
          f_distance = pythagoras(fabs(m_x - nearest_f.get_x()), fabs(m_y - nearest_f.get_y()));
        }
      }
    }
  }
  if(nearest_f.get_type() != agent_type::none){
    double x = -(0.01 * (m_x - nearest_f.get_x()));
    x = std::max(-0.05, std::min(x, 0.05));
    m_x += x;
    double y = -(0.01 * (m_y - nearest_f.get_y()));
    y = std::max(-0.05, std::min(y, 0.05));
    m_y += y;
  }
}

void agent::attract_to_agent(game &g, agent_type type){
  agent near_a(agent_type::none, INFINITY, INFINITY);
  double distance = pythagoras(fabs(m_x - near_a.get_x()), fabs(m_y - near_a.get_y()));
  for(agent a : g.get_agents()){
    double distance_a = pythagoras(fabs(m_x - a.get_x()), fabs(m_y - a.get_y()));
    if(distance_a > 350) return;
    if(a.get_type() == type &&
      distance_a < distance){
        near_a = a;
        distance = pythagoras(fabs(m_x - near_a.get_x()), fabs(m_y - near_a.get_y()));
    }
  }
  if(near_a.get_type() != agent_type::none){
    double x = -(0.01 * (m_x - near_a.get_x()));
    x = std::max(-0.05, std::min(x, 0.05));
    m_x += x;
    double y = -(0.01 * (m_y - near_a.get_y()));
    y = std::max(-0.05, std::min(y, 0.05));
    m_y += y;
    return;
  }
}

void agent::process_events(game& g) { //!OCLINT NPath complexity too high
  move();

  move_to_food(g);

  if(m_type == agent_type::spider) attract_to_agent(g, agent_type::venus_fly_trap);

  if ((m_type == agent_type::grass || m_type == agent_type::tree
      || m_type == agent_type::cow) && g.allow_damage())  reproduce_agents(g, m_type);

  if (m_type == agent_type::grass
    || m_type == agent_type::cactus
    || m_type == agent_type::tree
  ) damage_near_grass(g, m_type);

   //TODO is depth suitable for agent
  if (will_drown(m_type)
    && !get_on_tile_type(g, *this).empty()
    && get_on_tile_type(g, *this).front() == tile_type::water)
  {
    m_stamina -= 0.2;
  }

  if (g.get_n_ticks() % 100 == 0)
    eat(g);

  if(m_type != agent_type::bird && !is_on_tile(g, *this))
  {
    m_health = 0.0;
  }

  if(m_type == agent_type::fish || m_type == agent_type::whale){
    for(tile& t: g.get_tiles()){
      if(is_on_specific_tile(*this, t) && t.get_type() != tile_type::water){
        m_health -= 0.01;
      }
    }
  }

  if(m_type == agent_type::corpse && corpse_ticks == -1){
    corpse_ticks = g.get_n_ticks();
  }
  if(m_type == agent_type::corpse && corpse_ticks + 300 < g.get_n_ticks()){
    unsigned int n = static_cast<unsigned int>(count_n_agents(g));
    for(unsigned int i = 0; i < n; i++){
      if(g.get_agents()[i] == *this){
        g.get_agents()[i] = g.get_agents().back();
        g.get_agents().pop_back();
      }
    }
  }
}

void agent::reproduce_agents(game& g, agent_type type) { //!OCLINT indeed to complex, but get this merged first :-)

  if(is_plant(type)){
    const double rand = ((std::rand() % 10) + 26) / 1000.0; // 20 extra for the grass self-damage
    // Grow
    m_health += rand;
  }

  if ((m_type == agent_type::grass && m_health > 100.0) ||
      (m_type == agent_type::tree && m_health > 500.0) ||
      (m_type == agent_type::cow && m_health > 100.0) ||
      (m_type == agent_type::cactus && m_health > 100.0))
  {
    //Random fractions, from 0.0 to 1.0
    const double f_parent{static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)};
    const double f_kid{static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)};
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
    double f_x{static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)};
    double f_y{static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)};
    assert(f_x >= 0.0 && f_x < 1.0);
    assert(f_y >= 0.0 && f_y < 1.0);
    double new_x{m_x + (((f_x * 2.0) - 1.0) * max_distance)};
    double new_y{m_y + (((f_y * 2.0) - 1.0) * max_distance)};

    agent new_agent(type, new_x, new_y, health_kid);
    std::vector<tile> t = get_current_tile(g, new_agent);
    while(t.empty()
        || !is_on_tile(g, new_agent)
        || get_on_tile_type(g, new_agent).at(0) == tile_type::water
        || !is_on_specific_tile(new_agent.get_x() - 6, new_agent.get_y() - 6, t.front())
        || !is_on_specific_tile(new_agent.get_x() + 18, new_agent.get_y() + 18, t.front())
    )
    {
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
      }
    }
    g.add_agents( { new_agent } );
    m_health = health_parent;
  }
}

void agent::damage_near_grass(game &g, agent_type type)
{
  const double max_distance { pythagoras(32.0, 32.0) };

  const double max_damage { 17.5/1000.0 };

  std::vector <agent> all_agents{ g.get_agents() };

  for (agent& current_agent : all_agents)
  {
    double delta = pythagoras(abs(current_agent.get_x() - m_x), abs(current_agent.get_y() - m_y));
    if (current_agent.get_type() == type &&
         delta <= max_distance
       )
    {
        double rate = 1 - delta / max_distance;
        double damage = max_damage * rate;
        m_health -= damage;
    }
  }
}

std::vector<agent> create_default_agents() noexcept //!OCLINT indeed too long
{
  std::vector<agent> agents;
  {
    agent a1(agent_type::chameleon);
    move_agent_to_tile(a1, -3, 0);
    agents.push_back(a1);
  }
  {
    agent a1(agent_type::cow);
    move_agent_to_tile(a1, 0, 0);
    agents.push_back(a1);
    agent a2(agent_type::cow, 40, 70);
    move_agent_to_tile(a2, 0, 0);
    agents.push_back(a2);
    agent a3(agent_type::grass, 70, 40, 50 + std::rand() / (RAND_MAX / (100 - 50 + 1) + 1));
    move_agent_to_tile(a3, 0, 0);
    agents.push_back(a3);
    agent a4(agent_type::sunflower, 42, 112);
    agent a5(agent_type::foxgloves, 60, 70);
    move_agent_to_tile(a4, 0, 0);
    agents.push_back(a4);
    move_agent_to_tile(a5, 0, 0);
    agents.push_back(a5);
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
    agent a4(agent_type::worm, 50, 130);
    move_agent_to_tile(a4, 1, 0);
    agents.push_back(a4);
  }
  {
    agent a1(agent_type::crocodile, 30, 160);
    move_agent_to_tile(a1, 0, 2);
    agents.push_back(a1);
    agent a2(agent_type::snake, 50, 15);
    move_agent_to_tile(a2, 0, 2);
    agents.push_back(a2);
    agent a3(agent_type::venus_fly_trap, 30, 160, 1000);
    move_agent_to_tile(a3, 0, 2);
    agents.push_back(a3);
    agent a4(agent_type::cactus, 10, 15, 100);
    move_agent_to_tile(a4, 0, 2);
    agents.push_back(a4);
    agent a5(agent_type::cactus, 30, 120, 100);
    move_agent_to_tile(a5, 0, 2);
    agents.push_back(a5);
  }
  {
    agent a1(agent_type::crocodile);
    move_agent_to_tile(a1, 2, 1);
    agents.push_back(a1);
    agent a2(agent_type::grass, 0, 0, 50 + std::rand() / (RAND_MAX / (100 - 50 + 1) + 1));
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
    agent a3(agent_type::octopus, 50, 70);
    move_agent_to_tile(a3, 3, 2);
    agents.push_back(a3);
  }
  {
    agent a1(agent_type::whale);
    move_agent_to_tile(a1, 3, 2);
    agents.push_back(a1);
  }
  {
    agent a1(agent_type::whale);
    move_agent_to_tile(a1, 3, 2);
    agents.push_back(a1);
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
    agent a1(agent_type::grass, 0, 0, 50 + std::rand() / (RAND_MAX / (100 - 50 + 1) + 1));
    move_agent_to_tile(a1, 1, -1);
    agents.push_back(a1);
    agent a2(agent_type::giraffe, 10, 20);
    move_agent_to_tile(a2, 1, -1);
    agents.push_back(a2);
    //agent a3(agent_type::lion, 120, 20);
    //move_agent_to_tile(a3, 1, -1);
    //agents.push_back(a3);
    agent a4(agent_type::crocodile, 180, 20);
    move_agent_to_tile(a4, 1, -1);
    agents.push_back(a4);
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
    agent a8(agent_type::bird, 75, 150);
    move_agent_to_tile(a8, 4, -1);
    agents.push_back(a8);
  }
  {
    agent a1(agent_type::tree, 90, 170);
    move_agent_to_tile(a1, 0, -2);
    agents.push_back(a1);
    agent a2(agent_type::squirrel, 90, 150);
    move_agent_to_tile(a2, 0, -2);
    agents.push_back(a2);
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

bool will_drown(agent_type a) { //!OCLINT can't be simpler
  switch (a) {
    case agent_type::plankton:
      return false;
  case agent_type::worm:
    return false;
    case agent_type::bird:
      return false;
    case agent_type::cow:
      return true;
    case agent_type::giraffe:
      return true;
    case agent_type::crocodile:
      return false;
    case agent_type::chameleon:
      return true;
    case agent_type::fish:
      return false;
    case agent_type::whale:
      return false;
    case agent_type::goat:
      return true;
    case agent_type::spider:
      return true;
    case agent_type::octopus:
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

sf::Vector2i get_depth(agent_type a){
    return sf::Vector2i(get_min_depth(a), get_max_depth(a));
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
    assert(!is_plant(agent_type::none));
    assert(!is_plant(agent_type::octopus));
    assert( is_plant(agent_type::plankton));
    assert(!is_plant(agent_type::snake));
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
    // TODO add assert is on tile
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
  // A bird moves
  {
    game g;
    std::srand(314);
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::bird, x, y);
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
  // Venus Fly Trap does not move
  {
    game g;
    const double x{12.34};
    const double y{56.78};
    agent a(agent_type::venus_fly_trap, x, y);
    assert(is_on_tile(g, a));
    a.move();
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
    game g({tile(0, 0, 0, 100, 100, 0, tile_type::grassland)}, { agent(agent_type::cow) } );
    g.set_allow_spawning(false);
    assert(!g.get_agents().empty());
    // Wait until cow starves
    while (g.get_agents()[0].get_type() != agent_type::corpse)
    {
      g.process_events();
    }
  }
  //Grass grows
  {
    game g(create_test_default_tiles(), { agent(agent_type::grass) } );
    assert(!g.get_agents().empty());
    const auto health_before = g.get_agents()[0].get_health();
    // Grow one turn
    g.process_events();
    const auto health_after = g.get_agents()[0].get_health();
    assert(health_after > health_before);
  }
  //Trees grow
  {
    game g(create_test_default_tiles(), { agent(agent_type::tree) } );
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
    assert(g.get_agents()[0].get_type() == agent_type::corpse); //!OCLINT accepted idiom
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
      create_test_default_tiles(),
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
  //Crocodiles eat cows
  {
    const double cow_health{5.0};
    game g(
      create_test_default_tiles(),
      {
        agent(agent_type::cow, 0.0, 0.0, cow_health),
        agent(agent_type::crocodile  , 0.0, 0.0, 10.0)
      }
    );
    assert(g.get_agents()[0].get_health() == cow_health);
    double crocodile_stamina = g.get_agents()[1].get_stamina();
    g.process_events();
    //Grass is eaten ...
    assert(g.get_agents()[0].get_health() < cow_health);
    //Cow is fed ...
    assert(g.get_agents()[1].get_stamina() > crocodile_stamina);
  }
  //Fish die when on land
  {
    game g({ tile(0, 0, 0, 2, 2, 0, tile_type::grassland) }, { agent(agent_type::fish) } );
    assert(!g.get_agents().empty());
    //Choke fish
    while (g.get_agents()[0].get_type() != agent_type::corpse)
    {
      g.process_events();
    }
  }
  //octopus die when on land
  {
    game g({ tile(0, 0, 0, 2, 2, 0, tile_type::grassland) }, { agent(agent_type::octopus) } );
    assert(!g.get_agents().empty());
    //Choke octopus
    while (g.get_agents()[0].get_type() != agent_type::corpse)
    {
      g.process_events();
    }
  }
  // Agents drown
  {
    game g({tile(0,0,0,3,3,10,tile_type::water)},
           {agent(agent_type::cow, 10, 10),
            agent(agent_type::fish, 10, 10)});
    double cow_before = g.get_agents()[0].get_stamina();
    double fish_before = g.get_agents()[1].get_stamina();
    g.process_events();
    double delta_cow = cow_before - g.get_agents()[0].get_stamina();
    double delta_fish = fish_before - g.get_agents()[1].get_stamina();
    assert(delta_fish < delta_cow);
  }
  //grass grows gradually
  {
    game g({tile(0,0,0,3,3,10,tile_type::grassland)},
           {agent(agent_type::grass, 10, 10),
            agent(agent_type::grass, 200, 200)});
    const auto prev_grass_health1 = g.get_agents()[0].get_health();
    const auto prev_grass_health2 = g.get_agents()[1].get_health();
    for(int i = 0; i < 10; i++){
      g.process_events();
    }
    const auto after_grass_health1 = g.get_agents()[0].get_health();
    const auto after_grass_health2 = g.get_agents()[1].get_health();
    const auto grass1_delta = after_grass_health1 - prev_grass_health1;
    const auto grass2_delta = after_grass_health2 - prev_grass_health2;
    assert(grass1_delta != grass2_delta);
  }
  //grass has different health when its duplicated
  {
    game g({tile(0,0,0,3,3,10,tile_type::grassland)},
           {agent(agent_type::grass, 10, 10, 100)});
    const auto prev_health = g.get_agents()[0].get_health();
    g.process_events();
    assert(g.get_agents().size() == 2);
    const auto after_health = g.get_agents()[0].get_health();
    const auto second_grass_health = g.get_agents()[1].get_health();
    assert(prev_health != after_health);
    assert(after_health != second_grass_health);
  }
  //get depth test
  {
    assert(get_depth(agent_type::fish) == sf::Vector2i(0, 50));
  }
  //a cow walks to grass when its close
  {
    game g(create_test_default_tiles(),
           {agent(agent_type::cow, 0, 0, 100),
            agent(agent_type::grass, 100, 100, 100)});
    double cow_prev_posX = g.get_agents()[0].get_x();
    double cow_prev_posY = g.get_agents()[0].get_y();
    double distanceX = g.get_agents()[1].get_x() - g.get_agents()[0].get_x();
    double distanceY = g.get_agents()[1].get_y() - g.get_agents()[0].get_y();
    //move the cow 100 times
    for(int i = 0; i < 1000; i++){
      g.process_events();
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
  //Grass damages nearby grasses
  {
    //agent(const agent_type type, const double x = 0.0, const double y = 0.0,
    //      const double health = 1.0,  const double direction = 0.0);
    game g({tile(0,0,0,3,3,10,tile_type::grassland)},
           {agent(agent_type::grass, 10, 10, 10),
            agent(agent_type::grass, 10, 10, 10)});
    // Make two grass patches near each other.
    const double prev_grass_health1 = g.get_agents()[0].get_health();
    const double prev_grass_health2 = g.get_agents()[1].get_health();
    // Check their current health.

    for(int i = 0; i < 20; i++){
      g.process_events();
    }
    // Damage time.

    const double after_grass_health1 = g.get_agents()[0].get_health();
    const double after_grass_health2 = g.get_agents()[1].get_health();
    // Check their health now.

    assert(after_grass_health1 < prev_grass_health1);
    assert(after_grass_health2 < prev_grass_health2);
    // See whether damage hath happened.
  }
  #define FIX_ISSUE_447
  #ifdef FIX_ISSUE_447
  //Cacti damage nearby cacti
  {
    // Make two plants next to each other.
    game g({tile(0,0,0,3,3,10,tile_type::desert)},
           {agent(agent_type::cactus, 10, 10, 10),
            agent(agent_type::cactus, 10, 10, 10)});

    // Check their initial health.
    const double prev_health1 = g.get_agents()[0].get_health();
    const double prev_health2 = g.get_agents()[1].get_health();

    // Damage time.
    for(int i = 0; i != 20; ++i){
      g.process_events();
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
    game g({tile(0,0,0,3,3,10,tile_type::grassland)},
           {agent(agent_type::cow, 10, 10, 150)});
    assert(g.get_agents().size() == 1);
    g.process_events();
    assert(g.get_agents().size() >= 2);
  }
  //#define FIX_ISSUE_540
  #ifdef FIX_ISSUE_540
  {
    assert(get_agent_reproduction_health(agent_type::cactus) == 100.0);
    assert(get_agent_reproduction_health(agent_type::cow) == 100.0);
    assert(get_agent_reproduction_health(agent_type::grass) == 100.0);
    assert(get_agent_reproduction_health(agent_type::tree) == 500.0);
  }
  #endif // FIX_ISSUE_540
}
