#ifndef AGENT_TYPE_H
#define AGENT_TYPE_H

#include <iosfwd>
/// The type a tile can be

enum class agent_type { none, cow, grass, fish, crocodile };

std::ostream& operator<<(std::ostream& os, const agent_type a) noexcept;
std::istream& operator>>(std::istream& is, agent_type& a) noexcept;

#endif // TILE_TYPE_H
