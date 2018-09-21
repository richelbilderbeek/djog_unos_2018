#include "game.h"
#include <QFile>
#include <SFML/Graphics.hpp>

int main(int argc, char ** argv)
{
  const std::vector<std::string> args(argv, argv + argc);

  //Re-create resource at executable's location
  { QFile f(":/nature_zen/resources/background_music.ogg"); f.copy("background_music.ogg"); }


  game g;
  return g.exec();
}
