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

private:
  ///Camera position
  float camera_x = 0;
  float camera_y = 0;

  int wheight, wwidth;

  std::vector<sf::RectangleShape> shapes;

};

///Extract the maximum number of times the sfml_game will be displayed
//STUB: returns 100 by default for now, new dafault must be -1
int extract_n_displayed_max(const std::vector<std::string>& args);

#endif // SFML_sfml_game_H
