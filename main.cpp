#include "agent.h"
#include "agent_type.h"
#include "game.h"
#include "sfml_title_screen.h"
#include "sfml_about_screen.h"
#include "sfml_game.h"
#include "sfml_game_delegate.h"
#include "sfml_resources.h"
#include "tile.h"
#include "tile_id.h"
#include <QFile>
#include <SFML/Graphics.hpp>
#include <cassert>

/// Nature Zen
/// @param argc the number of arguments Nature Zen's executable is called
///   with by the operating system.
/// Arguments are:
///   * '--music': run with music
///   * '--short': only run for a couple of seconds
///   * '--title': show the title screen
///   * '--menu': show the menu screen
///   * '--about': show the about screen
///   * '--spin': that's a secret...
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
  test_tile_id();
}
int show_sfml_title_screen() {
    sfml_title_screen ts;
    ts.exec();
    return 0;
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
int main(int argc, char **argv) //!OCLINT WARNING main function too long
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
    return show_sfml_title_screen();
  }
  if (std::count(std::begin(args), std::end(args), "--menu"))
  {
    return show_sfml_menu_screen();
  }
  if (std::count(std::begin(args), std::end(args), "--about"))
  {
    return show_sfml_about_screen();
  }

  int close_at{-1};

  if (std::count(std::begin(args), std::end(args), "--short"))
  {
    close_at = 600;
  }

  std::vector<tile> tiles;
  std::vector<agent> agents;

  if (std::count(std::begin(args), std::end(args), "--spin"))
  {
    tiles.push_back(tile(2,-1,0,4,6,0,tile_type::mountains));
    tiles.push_back(tile(0,-1,0,2,6,0,tile_type::grassland));
    tiles.push_back(tile(-2.2,-1,0,0.2,1,0,tile_type::nonetile));
    tiles.push_back(tile(-2.2,1,0,0.2,1,0,tile_type::nonetile));
    tiles.push_back(tile(-2.2,3,0,0.2,1,0,tile_type::nonetile));
    agents.push_back(agent(agent_type::spider,50));
  } else {
    tiles = create_default_tiles();
    agents = create_default_agents();
  }

  sfml_game g(800, 600, sfml_game_delegate(close_at), tiles, agents);

  if (!std::count(std::begin(args), std::end(args), "--music"))
  {
    g.stop_music();
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
