#ifndef SFML_ABOUT_SCREEN_H
#define SFML_ABOUT_SCREEN_H

#include <SFML/Graphics.hpp>

class sfml_about_screen {
public:
  sfml_about_screen(const int close_at = -1);

  void close();

  void exec();

private:

  sf::RenderWindow& m_window;

  sf::Text m_text;

  int m_close_at;

  sf::Font m_font;

};

#endif // SFML_ABOUT_SCREEN_H
