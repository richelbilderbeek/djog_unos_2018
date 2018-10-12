#ifndef ABOUT_SCREEN_H
#define ABOUT_SCREEN_H

#include <SFML/Graphics.hpp>

class about_screen
{
public:
    about_screen(
      const int window_width = 1000,
      const int window_height = 600
    );
    void close();
    void display();
    sf::RenderWindow aboutwindow;
    sf::Text abouttext;
};

#endif // ABOUT_SCREEN_H
