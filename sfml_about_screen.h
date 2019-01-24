#ifndef SFML_ABOUT_SCREEN_H
#define SFML_ABOUT_SCREEN_H

#include <SFML/Graphics.hpp>

class sfml_about_screen {
public:
  sfml_about_screen(const int close_at = -1,
                    const int window_width = 1000,
                    const int window_height = 600);
  void close();
  void exec();
private:
  RenderWindow m_about_window;
  Text m_about_text;
  int m_close_at;
};

#endif // SFML_ABOUT_SCREEN_H
