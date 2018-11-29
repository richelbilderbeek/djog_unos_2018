#include "sfml_about_screen.h"
//#include "Text.hpp"
//#include <SFML/Graphics.hpp>
#include <QFile>
#include <cassert>

sfml_about_screen::sfml_about_screen(const int window_width, const int window_height)
    : aboutwindow(sf::VideoMode(static_cast<unsigned int>(window_width),
                                static_cast<unsigned int>(window_height)),
                  "About") {
  // Haha done everything already :-)
}

void sfml_about_screen::close()
{
  aboutwindow.close();
}

void sfml_about_screen::exec()
{
  while (aboutwindow.isOpen())
  {
    sf::Event event;
    while (aboutwindow.pollEvent(event))
    { //!OCLINT indeed an empty while statement
      // Indeed empty
    }

    sf::Font font;
    assert(QFile::exists("OpenSans.ttf"));
    font.loadFromFile("OpenSans.ttf");
    aboutwindow.clear(sf::Color::Green); // Clear the window with black color
    sfml_about_screen::abouttext.setFont(font);
    sfml_about_screen::abouttext.setString("About lalala..");
    sfml_about_screen::abouttext.setCharacterSize(24); // in pixels, not points!

    // set the color
    #if(SFML_VERSION_MINOR > 1)
    sfml_about_screen::abouttext.setFillColor(sf::Color::Red);
    #else
    //Only relevant for Travis
    sfml_about_screen::abouttext.setColor(sf::Color::Red);
    #endif
    aboutwindow.draw(sfml_about_screen::abouttext);
    aboutwindow.display();
  }
}
