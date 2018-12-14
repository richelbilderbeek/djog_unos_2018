#include "sfml_about_screen.h"
//#include "Text.hpp"
//#include <SFML/Graphics.hpp>
#include <QFile>
#include <cassert>

sfml_about_screen::sfml_about_screen(const int window_width, const int window_height)
    : m_about_window(sf::VideoMode(static_cast<unsigned int>(window_width),
                                static_cast<unsigned int>(window_height)),
                  "About") {
  // Haha done everything already :-)
}

void sfml_about_screen::close()
{
  m_about_window.close();
}

void sfml_about_screen::exec()
{
  while (m_about_window.isOpen())
  {
    sf::Event event;
    while (m_about_window.pollEvent(event))
    { //!OCLINT indeed an empty while statement
      // Indeed empty
    }

    sf::Font font;
    assert(QFile::exists("font.ttf"));
    font.loadFromFile("font.ttf");
    m_about_window.clear(sf::Color::Green); // Clear the window with black color
    sfml_about_screen::m_about_text.setFont(font);
    sfml_about_screen::m_about_text.setString("About lalala..");
    sfml_about_screen::m_about_text.setCharacterSize(24); // in pixels, not points!

    // set the color
    #if(SFML_VERSION_MINOR > 1)
    sfml_about_screen::m_about_text.setFillColor(sf::Color::Red);
    #else
    //Only relevant for Travis
    m_about_text.setColor(sf::Color::Red);
    #endif
    m_about_window.draw(sfml_about_screen::m_about_text);
    m_about_window.display();
  }
}
