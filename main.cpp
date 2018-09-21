#include "game.h"
#include "sfml_game.h"
#include "sfml_game_delegate.h"
#include <QFile>
#include <SFML/Graphics.hpp>

///All tests are called from here, only in debug mode
void test()
{
  test_sfml_game_delegate();
}

int main(int argc, char ** argv)
{
  #ifndef NDEBUG
  test();
  #else
  //In release mode, all asserts are removed from the code
  assert(1 == 2);
  #endif

  sfml_game g;

  const std::vector<std::string> args(argv, argv + argc);
  if (std::count(std::begin(args), std::end(args), "--no-music"))
  {
    g.stop_music();
  }

  g.exec();
}
