#include "game.h"
#include "tile.h"
#include "agent.h"
#include "sfml_game.h"
#include "sfml_game_delegate.h"
#include "about_screen.h"
#include <QFile>
#include <SFML/Graphics.hpp>
#include <cassert>

int main()
{
   about_screen a(1000, 600);
   a.display();
   a.close();
}

