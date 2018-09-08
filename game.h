#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class game
{
public:
  game();

  ///Run the game until the user closes its, or an
  ///exception is thrown
  ///@return the error code (0 = OK, others are errors)
  int exec();
private:

  ///The angle of the rectangle, just to have a member variable
  double m_angle;

  ///The window the game is rendered to
  sf::RenderWindow m_window;

  ///Display all shapes on the window
  void display();

  ///Timer, physics, bullets moving, etc.
  ///Everything except user input.
  void process_events();

  ///Key and mouse handling
  void process_input();

};

#endif // GAME_H
