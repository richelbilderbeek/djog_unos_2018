#include "agent_type.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <string>

std::vector<agent_type> collect_all_agent_types()
{
  return
  {
    agent_type::cow,
    agent_type::crocodile,
    agent_type::bacterium,
    agent_type::fish,
    agent_type::grass,
    agent_type::tree,
    agent_type::bird,
    agent_type::spider,
    agent_type::goat,
    agent_type::none
  };
}


void test_agent_type() //!OCLINT testing functions may be long
{
  {
    static_assert(agent_type::cow != agent_type::bacterium, "bacterium must exist");
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
    assert(std::count(std::begin(v), std::end(v), agent_type::tree) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::goat) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::none) == 1);
    #endif
  }
  {
    const std::vector<agent_type> v = collect_all_agent_types();
    for (agent_type t : v)
    {
      std::string s = to_str(t);
      agent_type u = to_agent(s);
      assert(t == u);
    }
  }
  //Collect all agent_types
  {
    const std::vector<agent_type> v = collect_all_agent_types();
    assert(std::count(std::begin(v), std::end(v), agent_type::cow) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::crocodile) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::fish) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::grass) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::tree) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::goat) == 1);
    assert(std::count(std::begin(v), std::end(v), agent_type::none) == 1);
  }
}

std::string to_str(agent_type a) //!OCLINT cannot be simpler
{
  switch (a) {
    case agent_type::bacterium:
      return "bacterium";
    case agent_type::bird:
      return "bird";
    case agent_type::cow:
      return "cow";
    case agent_type::crocodile:
      return "crocodile";
    case agent_type::grass:
      return "grass";
    case agent_type::fish:
      return "fish";
    case agent_type::tree:
      return "tree";
    case agent_type::goat:
      return "goat";
    case agent_type::spider:
      return "spider";
    default:
      assert(a == agent_type::none); //!OCLINT accepted idiom
      return "none";
  }
}

agent_type to_agent(std::string str) //!OCLINT cannot be simpler
{
  if (str == "cow") return agent_type::cow;
  if (str == "bacterium") return agent_type::bacterium;
  if (str == "grass") return agent_type::grass;
  if (str == "fish") return agent_type::fish;
  if (str == "crocodile") return agent_type::crocodile;
  if (str == "tree") return agent_type::tree;
  if (str == "goat") return agent_type::goat;
  if (str == "bird") return agent_type::bird;
  if (str == "spider") return agent_type::spider;
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
