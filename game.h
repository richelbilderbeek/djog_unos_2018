#ifndef GAME_H
#define GAME_H

#include <sfml_game.h>
#include <SFML/Graphics.hpp>


class game
{

public:
    ///Constructor
    game();

private:

  ///Timer, physics, bullets moving, etc.
  ///Everything except user input.
  void process_events();
};

#endif // GAME_H
