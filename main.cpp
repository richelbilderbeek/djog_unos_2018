#include "game.h"
#include <QFile>
#include <SFML/Graphics.hpp>

int main()
{
  //Re-create resource at executable's location
  { QFile f(":/nature_zen/resources/background_music.ogg"); f.copy("background_music.ogg"); }


  game g;
  return g.exec();
}
