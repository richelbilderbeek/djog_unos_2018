#include "game.h"
#include "tile.h"
#include "agent.h"
#include "sfml_game.h"
#include "sfml_game_delegate.h"
#include "sfml_about_screen.h"
#include <QFile>
#include <SFML/Graphics.hpp>
#include <cassert>

int main()
{
  //#define FIX_ISSUE_217
  #ifdef FIX_ISSUE_217
  sfml_about_screen a(1000, 600);
  a.exec();
  #endif // FIX_ISSUE_217
}
