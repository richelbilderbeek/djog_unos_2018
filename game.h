#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class game
{
public:
  ///Create a game from the command-line arguments
  game(const std::vector<std::string>& args = {});

  ///Get how many times the game has been displayed on screen.
  ///Will be approximately 60 times per second.
  int get_n_displayed() const noexcept { return m_n_displayed; }

  ///Get how many times the game has been displayed on screen.
  ///Will be approximately 60 times per second.
  int get_n_displayed_max() const noexcept { return m_n_displayed_max; }

  ///Run the game until the user closes its, or an
  ///exception is thrown
  ///@return the error code (0 = OK, others are errors)
  int exec();
private:

  ///The angle of the rectangle, just to have a member variable
  double m_angle{0.0};

  ///The number of times the game is displayed on screen
  ///Should be approx 60 times per second
  int m_n_displayed{0};

  ///The number of times the game is displayed before it closes.
  ///This is usefull in testing.
  ///Will be negative if the game has no time limit
  const int m_n_displayed_max;

  ///Screen Width and Height
  int width = 800;
  int height = 600;

  ///Camera position
  float camera_x = 0;
  float camera_y = 0;

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

///Extract the maximum number of times the game will be displayed
//STUB: returns 100 by default for now, new dafault must be -1
int extract_n_displayed_max(const std::vector<std::string>& args);

#endif // GAME_H
