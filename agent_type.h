#ifndef AGENT_TYPE_H
#define AGENT_TYPE_H

#include <iosfwd>
#include <vector>

/// The type an agent can be
enum class agent_type {
    none,
    plankton,
    cow,
    grass,
    sun_flower,
    fish,
    crocodile,
    sunflower,
    squirrel,
    tree,
    goat,
    snake,
    bird,
    spider,
    octopus,
    worm,
    whale,
    lion,
    giraffe,
    venus_fly_trap,
    foxgloves,
    cactus
};

/// Get all the agent types in a std::vector
std::vector<agent_type> collect_all_agent_types();
std::string to_str(agent_type a);
agent_type to_agent(std::string);

///Tests the agent_type
void test_agent_type();

std::ostream& operator<<(std::ostream& os, const agent_type a) noexcept;
std::istream& operator>>(std::istream& is, agent_type& a) noexcept;

#endif // TILE_TYPE_H
