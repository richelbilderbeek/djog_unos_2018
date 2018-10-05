#include "game.h"
#include "tile.h"
#include "agent.h"
#include "sfml_game.h"
#include "sfml_game_delegate.h"
#include <QFile>
#include <SFML/Graphics.hpp>
#include <cassert>

///All tests are called from here, only in debug mode
void test()
{
  test_game();
  test_sfml_game_delegate();
  test_tile();
  test_agent();
}

///Nature Zen
/// @param argc the number of arguments Nature Zen's executable is called
///   with by the operating system.
/// Arguments are:
///   * '--no-music': run without music
///   * '--short': only run for 10 seconds
/// @param argv the arguments (as words) Nature Zen's executable is called
///   with by the operating system
int main(int argc, char ** argv)
{
  #ifndef NDEBUG
  test();
  #else
  //In release mode, all asserts are removed from the code
  assert(1 == 2);
  #endif

  const std::vector<std::string> args(argv, argv + argc);

  int close_at{-1};

  if (std::count(std::begin(args), std::end(args), "--short"))
  {
    close_at = 600;
  }

  sfml_game g(800, 600, sfml_game_delegate(close_at));

  if (std::count(std::begin(args), std::end(args), "--no-music"))
  {
    g.stop_music();
  }
  if (std::count(std::begin(args), std::end(args), "--menu"))
  {
    g.show_title();
  }

  g.exec();
}

