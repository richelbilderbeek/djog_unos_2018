#include "agent_type.h"
#include "string"

std::string to_str(agent_type t)
{
  switch (t) {
    case agent_type::cow:
        return "cow";

    case agent_type::grass:
        return "grass";

    default:
      return "none";
  }
}

agent_type to_agent(std::string str)
{
  if (str == "cow") return agent_type::cow;
  if (str == "grass") return agent_type::grass;
  return agent_type::none;
}

std::ostream& operator <<(std::ostream& os, const agent_type a) noexcept {
    os << to_str(a);
    return os;
}

std::istream& operator>>(std::istream& is, agent_type& a) noexcept {
    std::string s;
    is >> s;
    a = to_agent(s);
    return is;
}
