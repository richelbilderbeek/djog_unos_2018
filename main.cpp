#include "agent.h"
#include "agent_type.h"
#include "biology.h"
#include "game.h"
#include "sfml_title_screen.h"
#include "sfml_about_screen.h"
#include "sfml_gameover_screen.h"
#include "sfml_game.h"
#include "sfml_game_delegate.h"
#include "sfml_load_screen.h"
#include "sfml_resources.h"
#include "sfml_window_manager.h"
#include "tile.h"
#include "tile_id.h"
#include "sfml_text_input.h"
#include "sfml_scroll_box.h"
#include "sfml_load_screen.h"
#include "sound_type.h"
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
///   * '--short [n]': only run for [n] ticks
///   * '--title': show the title screen
///   * '--menu': show the menu screen
///   * '--about': show the about screen
///   * '--version': show the SFML version and quit
///   * '--spin': that's a secret...
///   * '--profiling': only run for a minute, for profiling
/// @param argv the arguments (as words) Nature Zen's executable is called
///   with by the operating system


/// All tests are called from here, only in debug mode
void test() {
  test_biology();
  test_agent();
  test_tile_type();
  test_tile();
  test_agent_type();
  test_sound_type();
  test_tile_id();
  //test_sfml_window_manager();
  test_normal_char();
  test_game();
  test_game_state();
  test_sfml_resources();
  test_sfml_game();
  test_sfml_game_delegate();
//  test_sfml_load_screen();
}

///Start the game
///@param close_at_tick tick at which the game will be closed.
///   if close_at_tick equals minus one, the game runs indefinitely
///@param music will there be music?
///@param tiles starting tiles
///@param agents initial agents
///@param spawning will agents spawn?
///@param damage can agents damage one another?
///@param score is the game score being tracked? If not, the player will
///   never die
///@return closing status, which is zero if everything went OK
int start_sfml_game(
  const int close_at_tick,
  bool music,
  bool sounds,
  std::vector<tile> tiles,
  std::vector<agent> agents,
  bool spawning,
  bool damage,
  bool score
) {
  //std::clog << "Create an sfml_game\n";
  sfml_game g(sfml_game_delegate(close_at_tick, spawning, damage, score), tiles, agents);
  if (!music) g.stop_music();
  if (!sounds) g.stop_sounds();

  //std::clog << "Execute an sfml_game\n";
  g.exec();

  //std::clog << "We're done with the sfml_game\n";
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

int show_sfml_load_screen()
{
  sfml_load_screen ls;
  ls.exec();
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
  //std::clog << "==========\n";
  //std::clog << "Nature Zen\n";
  //std::clog << "==========\n";
#ifndef NDEBUG
  test_ref();
  test();
#else
  // In release mode, all asserts are removed from the code
  assert(1 == 2);
#endif
  
  const std::vector<std::string> args(argv, argv + argc);
  
  //std::clog << "Get the user's name\n";
  std::string user = "";
#ifdef WIN32
  user = getenv("USERNAME");
#endif
#ifdef __linux__
  user = system("whoami");
#endif
  if (user != "") {
    std::clog << "Current user: " << user << "\n" << std::endl;
  }

  //----------------------------------------------------------------------------
  //Things with early exits
  //----------------------------------------------------------------------------
  //std::clog << "Processing CLI\n";
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
      << std::endl
    ;
    return 0; // 0: everything went OK
  }

  bool music = false;
  bool sounds = false;
  bool spawning = true;
  bool damage = true;
  bool score = true;

  if (std::count(std::begin(args), std::end(args), "--no-music"))
  {
    music = true;
  }
  if (std::count(std::begin(args), std::end(args), "--sounds"))
  {
    sounds = true;
  }

  int close_at{-1};
  
  if (std::count(std::begin(args), std::end(args), "--short"))
  {
    //std::clog << "Process '--short' CLI option\n";
    close_at = 600;
    assert(std::find(std::begin(args), std::end(args), "--short") != std::end(args));
    if (std::find(std::begin(args), std::end(args), "--short") + 1 != std::end(args))
    {
      const std::string s{
        *(std::find(std::begin(args), std::end(args), "--short") + 1)
      };
      if(s.at(0) != '-'){
        close_at = std::atoi(s.c_str());
      }
    }
    //The '--short' setting is used on Travis for debugging and
    //should jump straight to the game
    sfml_window_manager::get().set_state(game_state::playing);
  }
  else if (std::count(std::begin(args), std::end(args), "--profiling")){
    //std::clog << "Process '--profiling' CLI option\n";
    close_at = 10000;
    sfml_window_manager::get().set_state(game_state::playing);
  }
  else if (std::count(std::begin(args), std::end(args), "--title"))
  {
    //std::clog << "Process '--title' CLI option\n";
    sfml_window_manager::get().set_state(game_state::titlescreen);
  }
  else if (std::count(std::begin(args), std::end(args), "--menu"))
  {
    //std::clog << "Process '--menu' CLI option\n";
    sfml_window_manager::get().set_state(game_state::menuscreen);
  }
  else if (std::count(std::begin(args), std::end(args), "--about"))
  {
    //std::clog << "Process '--about' CLI option\n";
    sfml_window_manager::get().set_state(game_state::aboutscreen);
  }
  else if (std::count(std::begin(args), std::end(args), "--game-over") ||
           std::count(std::begin(args), std::end(args), "--gameover")) {
    //std::clog << "Process '--gameover' CLI option\n";
    sfml_window_manager::get().set_state(game_state::gameover);
  }
  else if (std::count(std::begin(args), std::end(args), "--paused"))
  {
    //std::clog << "Process '--paused' CLI option\n";
    sfml_window_manager::get().set_state(game_state::paused);
  }
  else if (std::count(std::begin(args), std::end(args), "--save"))
  {
    //std::clog << "Process '--save' CLI option\n";
    sfml_window_manager::get().set_state(game_state::saving);
  }
  //std::clog << "Processed all CLI options\n";

  //Not realy to show settings, but to use the variables
  std::cout << "Settings\n"
            << "Close at : " << close_at << "\n"
            << "Music    : " << music << "\n"
            << "Sounds   : " << sounds << std::endl;

  std::vector<tile> tiles;
  std::vector<agent> agents;

//  if (std::count(std::begin(args), std::end(args), "--spin"))
//  {
//    tiles.push_back(tile(0,-112,0,90,0,tile_type::grassland));
//    agents.push_back(agent(agent_type::spider,50));
//  }
  if(std::count(std::begin(args), std::end(args), "--profiling")) {
    int agents_size = 10;
    int tiles_size = 10;
    if (std::find(std::begin(args), std::end(args), "--profiling") + 1 != std::end(args))
    {
      const std::string s{
        *(std::find(std::begin(args), std::end(args), "--profiling") + 1)
      };
      if(!s.empty() && s.at(0) != '-'){
        agents_size = std::atoi(s.c_str());
      }
    }
    if (std::find(std::begin(args), std::end(args), "--profiling") + 2 != std::end(args))
    {
      const std::string s{
        *(std::find(std::begin(args), std::end(args), "--profiling") + 2)
      };
      if(!s.empty() && s.at(0) != '-'){
        tiles_size = std::atoi(s.c_str());
      }
    }
    for(int i = 0; i < agents_size; i++){
      int type = random_int(1, 21);
      agent a(random_agent_type(type), i, i);
      agents.push_back(a);
    }
    for(int i = 0; i < tiles_size; i++){
      tile t(i * 112, i * 112, 0, 90, 0, tile_type::grassland);
      tiles.push_back(t);
    }
    spawning = false;
    damage = false;
    score = false;
  }
  else if(std::count(std::begin(args), std::end(args), "--god")) {
    //std::clog << "Process '--god' CLI option\n";
    score = false;
    tiles = create_test_default_tiles();
    agents = create_default_agents();
  }

  //A window should have opened up now
  assert(sfml_window_manager::get().get_window().isOpen());

  while (sfml_window_manager::get().get_window().isOpen()) {
    std::clog << "State: " << sfml_window_manager::get().get_state() << '\n';
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
      case game_state::saving:
      case game_state::paused:
      case game_state::shop:
      case game_state::playing:
        start_sfml_game(close_at, music, sounds, tiles, agents, spawning, damage, score);
        break;
      case game_state::gameover:
        show_sfml_gameover_screen(-1);
        break;
      case game_state::loading:
        show_sfml_load_screen();
        break;
    }
  }
}

