#include "agent_type.h"
#include "string"
#include <algorithm>
#include <cassert>
#include <string>

std::vector<agent_type> collect_all_agent_types()
{
  return
  {
        agent_type::cow,
        agent_type::crocodile,
        agent_type::bacteria,
        agent_type::fish,
        agent_type::grass,
        agent_type::none
  };
}


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
    #define FIX_ISSUE_204
    #ifdef FIX_ISSUE_204
    const std::vector<agent_type> v = collect_all_agent_types();
    assert(std::count(std::begin(v), std::end(v), agent_type::cow) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::crocodile) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::fish) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::grass) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::none) == 1);
    #endif
  }
  {
    //Uncomment if you want to run this test
    #define FIX_ISSUE_188
    #ifdef FIX_ISSUE_188
    const std::vector<agent_type> v = collect_all_agent_types();
    for (const agent_type t : v)
    {
       const std::string s = to_str(t);
      const agent_type u = to_agent(s);
      assert(t == u);
    }
    #endif // FIX_ISSUE_188
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

    case agent_type::none:
      return "none";
  }
  assert(!"Agent types aren't translated completely");
  return "none";
}

agent_type to_agent(std::string str)
{
  if (str == "cow") return agent_type::cow;
  if (str == "batteria") return agent_type::bacteria;
  if (str == "grass") return agent_type::grass;
  if (str == "fish") return agent_type::fish;
  if (str == "crocodile") return agent_type::crocodile;
  if (str == "none") return agent_type::none;
  assert(!"Agent types aren't translated completely");
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


bool operator==(agent_type lhs, agent_type rhs) noexcept{
    if (to_str(lhs) == to_str(rhs)) return true;
    else return false;

}
