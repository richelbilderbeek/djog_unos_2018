#include "about_screen.h"
//#include "Text.hpp"
//#include <SFML/Graphics.hpp>
#include <QFile>
#include <cassert>

about_screen::about_screen(const int window_width, const int window_height)
    : aboutwindow(sf::VideoMode(static_cast<unsigned int>(window_width),
                                static_cast<unsigned int>(window_height)),
                  "About") {
  // Haha done everything already :-)
  // Re-create font
  {
    QFile f(":/nature_zen/resources/OpenSans.ttf");
    f.copy("OpenSans.ttf");
    assert(QFile::exists("OpenSans.ttf"));
  }
}

void about_screen::close() { aboutwindow.close(); }

void about_screen::display() {
  while (aboutwindow.isOpen()) {
    sf::Event event;
    while (aboutwindow.pollEvent(event)) { //!OCLINT indeed an empty while statement
      // Haha!
    }

    sf::Font font;
    assert(QFile::exists("OpenSans.ttf"));
    font.loadFromFile("OpenSans.ttf");
    aboutwindow.clear(sf::Color::Green); // Clear the window with black color
    about_screen::abouttext.setFont(font);
    about_screen::abouttext.setString("About lalala..");
    about_screen::abouttext.setCharacterSize(24); // in pixels, not points!

    // set the color
    #if(SFML_VERSION_MINOR > 1)
    about_screen::abouttext.setFillColor(sf::Color::Red);
    #else
    about_screen::abouttext.setColor(sf::Color::Red);
    #endif
    aboutwindow.draw(about_screen::abouttext);
    aboutwindow.display();
  }
}
