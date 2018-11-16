#ifndef SFML_ABOUT_SCREEN_H
#define SFML_ABOUT_SCREEN_H

#include <SFML/Graphics.hpp>

class sfml_about_screen {
public:
  sfml_about_screen(const int window_width = 1000, const int window_height = 600);
  void close();
  void exec();
  sf::RenderWindow aboutwindow;
  sf::Text abouttext;
};

#endif // SFML_ABOUT_SCREEN_H
