#include "agent_type.h"

#include <algorithm>
#include <cassert>
#include <string>

void test_agent_type() //!OCLINT testing functions may be long
{
  {
    //#define FIX_ISSUE_224
    #ifdef FIX_ISSUE_224
    static_assert(agent_type::cow != agent_type::bacterium, "bacterium must exist");
    #endif
  }
  //Collect all agent_types
  {
    //#define FIX_ISSUE_204
    #ifdef FIX_ISSUE_204
    const std::vector<agent_type> v = collect_all_agent_types();
    assert(std::count(std::begin(v), std::end(v), agent_type::cow) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::crocodile) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::fish) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::grass) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::none) == 1);
    #endif
  }
}

std::string to_str(agent_type a)
{
  switch (a) {
    case agent_type::cow:
      return "cow";

    case agent_type::grass:
      return "grass";

    case agent_type::fish:
      return "fish";

    case agent_type::crocodile:
      return "crocodile";

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
    a = to_agent(s); //!OCLINT correct idiom in this context
    return is;
}
