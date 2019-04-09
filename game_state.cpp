#include "game_state.h"
#include <cassert>
#include <iostream>

std::string to_str(const game_state s) noexcept
{
  switch(s)
  {
    case game_state::aboutscreen: return "aboutscreen";
    case game_state::gameover: return "gameover";
    case game_state::loading: return "loading";
    case game_state::menuscreen: return "menuscreen";
    case game_state::paused: return "paused";
    case game_state::playing: return "playing";
    case game_state::saving: return "saving";
    case game_state::shop: return "shop";
    case game_state::titlescreen: return "titlescreen";
  }
  assert(!"Should not get here"); //!OCLINT accepted idiom
  return "";
}

std::ostream& operator<<(std::ostream& os, const game_state s)
{
  os << to_str(s);
  return os;
}
