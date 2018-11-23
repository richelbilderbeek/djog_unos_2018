#include "agent.h"
#include "agent_type.h"
#include "game.h"
#include "sfml_title_screen.h"
#include "sfml_about_screen.h"
#include "sfml_game.h"
#include "sfml_game_delegate.h"
#include "sfml_resources.h"
#include "tile.h"
#include <QFile>
#include <SFML/Graphics.hpp>
#include <cassert>

/// Nature Zen
/// @param argc the number of arguments Nature Zen's executable is called
///   with by the operating system.
/// Arguments are:
///   * '--no-music': run without music
///   * '--short': only run for 10 seconds
///   * '--menu': show the menu
///   * '--about': access about screen
/// @param argv the arguments (as words) Nature Zen's executable is called
///   with by the operating system


/// All tests are called from here, only in debug mode
void test() {
  test_sfml_resources();
  test_game();
  test_sfml_game();
  test_sfml_game_delegate();
  test_tile_type();
  test_tile();
  test_agent();
  test_agent_type();
}
int show_sfml_menu_screen() {
    sfml_menu_screen ms;
    ms.exec();
    return 0;
}
int show_sfml_about_screen() {
    sfml_about_screen as;
    as.exec();
    return 0;
}
int main(int argc, char **argv)
{
#ifndef NDEBUG
  test();
#else
  // In release mode, all asserts are removed from the code
  assert(1 == 2);
#endif

  const std::vector<std::string> args(argv, argv + argc);

  if (std::count(std::begin(args), std::end(args), "--title"))
  {
    sfml_title_screen ts;
    ts.exec();
    return 0;
  }

  int close_at{-1};

  if (std::count(std::begin(args), std::end(args), "--short"))
  {
    close_at = 600;
  }

  sfml_game g(800, 600, sfml_game_delegate(close_at));

  if (!std::count(std::begin(args), std::end(args), "--music"))
  {
    g.stop_music();
  }

  if (std::count(std::begin(args), std::end(args), "--menu"))
  {
    return show_sfml_menu_screen();
  }
  if (std::count(std::begin(args), std::end(args), "--about"))
  {
    return show_sfml_about_screen();
  }
  if (std::count(std::begin(args), std::end(args), "--version")) {
    std::cout
      << 'v' << SFML_VERSION_MAJOR
      << "." << SFML_VERSION_MINOR
      #if(SFML_VERSION_MINOR > 1)
      << "." << SFML_VERSION_PATCH
      #endif
    ;
  }
  g.exec();
}
