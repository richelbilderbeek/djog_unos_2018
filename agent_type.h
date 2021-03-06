#ifndef AGENT_TYPE_H
#define AGENT_TYPE_H

#include <iosfwd>
#include <vector>

/// The type an agent can be
enum class agent_type {
  bird,
  cactus,
  chameleon,
  cow,
  corpse,
  polar_bear,
  crocodile,
  fish,
  foxgloves,
  giraffe,
  goat,
  grass,
  lion,
  octopus,
  plankton,
  snake,
  spider,
  squirrel,
  sunflower,
  tree,
  venus_fly_trap,
  whale,
  worm
};

/// Get all the agent types in a std::vector
std::vector<agent_type> collect_all_agent_types();
std::string to_str(agent_type a);
agent_type to_agent(std::string);
agent_type random_agent_type(int type);

///Tests the agent_type
void test_agent_type();

std::ostream& operator<<(std::ostream& os, const agent_type a) noexcept;
std::istream& operator>>(std::istream& is, agent_type& a) noexcept;

#endif // TILE_TYPE_H
