// Always include the header of the unit first
#include "sfml_game_delegate.h"

#include <cassert>
#include <SFML/Window/Event.hpp>
#include "sfml_game.h"

sfml_game_delegate::sfml_game_delegate(const int close_at)
    : m_close_at{close_at} {}

bool sfml_game_delegate::do_actions(sfml_game &sg) {

  // Nothing to do?
  if (m_close_at < 0)
    return false;

  if (sg.get_n_displayed() % 2 == 0)
  {
    sg.select_random_tile();
  }
  if (sg.get_n_displayed() % 2 != 0)
  {
    sg.move_selected_tile_randomly();
  }

  // Close at a certain time
  if (sg.get_n_displayed() > m_close_at) {
    sg.close();
    // Did something
    return true;
  }

  // Done nothing
  return false;
}

void test_sfml_game_delegate() {
  // close_at must be stored correctly
  {
    const int close_at{42};
    sfml_game_delegate d(close_at);
    assert(d.get_close_at() == close_at);
  }
}
