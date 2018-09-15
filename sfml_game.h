#ifndef SFML_sfml_game_H
#define SFML_sfml_game_H

#include <SFML/Graphics.hpp>

class sfml_game
{
public:

   sfml_game();

  ///Get how many times the sfml_game has been displayed on screen.
  ///Will be approximately 60 times per second.
  int get_n_displayed() const noexcept { return m_n_displayed; }

  ///Get how many times the sfml_game has been displayed on screen.
  ///Will be approximately 60 times per second.
  int get_n_displayed_max() const noexcept { return m_n_displayed_max; }

  void add_shape(sf::Vector2f size, sf::Vector2f position, float rotation);

  ///Run the sfml_game until the user closes its, or an
  ///exception is thrown
  ///@return the error code (0 = OK, others are errors)
  int exec();
private:

  ///The angle of the rectangle, just to have a member variable
  double m_angle{0.0};

  ///The number of times the sfml_game is displayed on screen
  ///Should be approx 60 times per second
  int m_n_displayed{0};

  ///The number of times the sfml_game is displayed before it closes.
  ///This is usefull in testing.
  ///Will be negative if the sfml_game has no time limit
  const int m_n_displayed_max;

  ///Screen Width and Height
  int width = 800;
  int height = 600;

  ///Camera position
  float camera_x = 0;
  float camera_y = 0;

  ///The window the sfml_game is rendered to
  sf::RenderWindow m_window;

  ///Display all shapes on the window
  void display();

  ///Timer, physics, bullets moving, etc.
  ///Everything except user input.
  void process_events();

  ///Key and mouse handling
  void process_input();

  std::vector<sf::RectangleShape> shapes;

};

///Extract the maximum number of times the sfml_game will be displayed
//STUB: returns 100 by default for now, new dafault must be -1
int extract_n_displayed_max(const std::vector<std::string>& args);

#endif // SFML_sfml_game_H
