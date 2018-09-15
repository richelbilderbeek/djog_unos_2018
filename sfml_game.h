#ifndef SFML_sfml_game_H
#define SFML_sfml_game_H

#include <SFML/Graphics.hpp>

class sfml_game
{
public:

  sfml_game(int width, int height);

  void add_shape(sf::RectangleShape shape);

  ///Run the sfml_game until the user closes its, or an
  ///exception is thrown
  ///@return the error code (0 = OK, others are errors)
  int init();

  ///The window the sfml_game is rendered to
  sf::RenderWindow m_window;

  ///Display all shapes on the window
  void display();

  void move_camera(sf::Vector2f offset);

private:
  ///Camera position
  float camera_x = 0;
  float camera_y = 0;

  int wheight, wwidth;

  std::vector<sf::RectangleShape> shapes;

};

#endif // SFML_sfml_game_H
