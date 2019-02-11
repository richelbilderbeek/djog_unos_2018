#include "agent.h"
#include "agent_type.h"
#include "game.h"
#include "sfml_title_screen.h"
#include "sfml_about_screen.h"
#include "sfml_gameover_screen.h"
#include "sfml_game.h"
#include "sfml_game_delegate.h"
#include "sfml_resources.h"
#include "sfml_window_manager.h"
#include "tile.h"
#include "tile_id.h"
#include <QFile>
#include <typeinfo>
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
///   * '--version': show the SFML version and quit
///   * '--spin': that's a secret...
/// @param argv the arguments (as words) Nature Zen's executable is called
///   with by the operating system


/// All tests are called from here, only in debug mode
void test() {
  /*test_sfml_resources();
  test_game();
  test_sfml_game();
  test_sfml_game_delegate();
  test_tile_type();
  test_tile();
  test_agent();
  test_agent_type();
  test_tile_id();
  //test_sfml_window_manager();*/
}
int start_sfml_game(int ca, bool music,
                    std::vector<tile> tiles,
                    std::vector<agent> agents) {
  sfml_game g(sfml_game_delegate(ca), tiles, agents);
  if (!music) g.stop_music();
  g.exec();
  return 0;
}
int show_sfml_title_screen(int ca, bool music) {
  sfml_title_screen ts(ca);
  if (!music) ts.stop_music();
  ts.exec();
  return 0;
}
int show_sfml_menu_screen(int ca) {
  sfml_menu_screen ms(ca);
  ms.exec();
  return 0;
}
int show_sfml_about_screen(int ca) {
  sfml_about_screen as(ca);
  as.exec();
  return 0;
}
int show_sfml_gameover_screen(int ca) {
  sfml_gameover_screen gos(ca);
  gos.exec();
  return 0;
}

#include <functional>

void test_ref()
{
  int a = 1;
  std::vector<std::reference_wrapper<int>> v;
  v.push_back(std::ref(a));
  v.push_back(std::ref(a));
  assert(v[0] == v[1]);
  ++v[0];
  assert(v[0] == v[1]);
  ++v[1];
  assert(v[0] == v[1]);
}

int main(int argc, char **argv) //!OCLINT main too long
{
#ifndef NDEBUG
  test_ref();
  test();
#else
  // In release mode, all asserts are removed from the code
  assert(1 == 2);
#endif
  
  const std::vector<std::string> args(argv, argv + argc);

  //----------------------------------------------------------------------------
  //Things with early exits
  //----------------------------------------------------------------------------
  //Show the SFML version and quit
  if (std::count(std::begin(args), std::end(args), "--version")) {
    // Travis: 2.1
    // RuG: 2.3.2
    std::cout
      << "SFML version: " << SFML_VERSION_MAJOR
      << "." << SFML_VERSION_MINOR
      #if(SFML_VERSION_MINOR > 1)
      << "." << SFML_VERSION_PATCH
      #endif
      << '\n'
    ;
    return 0; // 0: everything went OK
  }

  bool music = false;

  if (std::count(std::begin(args), std::end(args), "--music"))
  {
    music = true;
  }

  int close_at{-1};
  
  if (std::count(std::begin(args), std::end(args), "--short"))
  {
    close_at = 600;
    sfml_window_manager::get().set_state(game_state::titlescreen);
  }
  else if (std::count(std::begin(args), std::end(args), "--title"))
  {
    sfml_window_manager::get().set_state(game_state::titlescreen);
  }
  else if (std::count(std::begin(args), std::end(args), "--menu"))
  {
    sfml_window_manager::get().set_state(game_state::menuscreen);
  }
  else if (std::count(std::begin(args), std::end(args), "--about"))
  {
    sfml_window_manager::get().set_state(game_state::aboutscreen);
  }
  else if (std::count(std::begin(args), std::end(args), "--game-over") ||
           std::count(std::begin(args), std::end(args), "--gameover")) {
    sfml_window_manager::get().set_state(game_state::gameover);
  }

  //Not realy to show settings, but to use the variables
  std::cout << "\nSettings\n"
            << "Close at : " << close_at << "\n"
            << "Music    : " << music << "\n";

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

  while (sfml_window_manager::get().get_window().isOpen()) {
    switch (sfml_window_manager::get().get_state()) {
      case game_state::titlescreen:
        show_sfml_title_screen(close_at, music);
        break;
      case game_state::menuscreen:
        show_sfml_menu_screen(close_at);
        break;
      case game_state::aboutscreen:
        show_sfml_about_screen(close_at);
        break;
      case game_state::playing:
        start_sfml_game(close_at, music, tiles, agents);
        break;
      case game_state::gameover:
        show_sfml_gameover_screen(close_at);
        break;
    }
  }
}

