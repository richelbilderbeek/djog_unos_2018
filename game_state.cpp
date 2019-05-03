#include "game_state.h"
#include <cassert>
#include <iostream>

void test_game_state()
{
  //#define FIX_ISSUE_578
  #ifdef FIX_ISSUE_578
  //Can collect all game states
  {
    const std::vector<game_state> v = collect_all_game_states();
    assert(!v.empty());
  }
  #endif //FIX_ISSUE_578
}

std::string to_str(const game_state s) noexcept //!OCLINT too complex indeed
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

std::vector<game_state> collect_all_game_states() {
  return {
      game_state::aboutscreen,
      game_state::gameover,
      game_state::loading,
      game_state::menuscreen,
      game_state::paused,
      game_state::playing,
      game_state::saving,
      game_state::shop,
      game_state::titlescreen
    };
}

std::ostream& operator<<(std::ostream& os, const game_state s)
{
  os << to_str(s);
  return os;
}
