#include "sfml_about_screen.h"
//#include "Text.hpp"
//#include <SFML/Graphics.hpp>
#include <QFile>
#include <cassert>

sfml_about_screen::sfml_about_screen(const int close_at,
                                     const int window_width,
                                     const int window_height)
    : m_about_window(VideoMode(static_cast<unsigned int>(window_width),
                                   static_cast<unsigned int>(window_height)),
                  "Nature Zen - About"), m_close_at{close_at} {
  // Haha done everything already :-)
}

void sfml_about_screen::close()
{
  m_about_window.close();
}

void sfml_about_screen::exec()
{
  if (m_close_at >= 0) close();
  while (m_about_window.isOpen())
  {
    Event event;
    while (m_about_window.pollEvent(event))
    {
      if(Keyboard::isKeyPressed(Keyboard::Space)){
        close();
      }
      switch (event.type) //!OCLINT too few branches, please fix
      {
        case Event::Closed:
            close();
            break;
        default:
            break;
      }
    }
    Font font;
    assert(QFile::exists("font.ttf"));
    font.loadFromFile("font.ttf");
    m_about_window.clear(Color::Green); // Clear the window with green color
    m_about_text.setFont(font);
    m_about_text.setString("About lalala..");
    m_about_text.setCharacterSize(24); // in pixels, not points!

    // set the color
    #if(SFML_VERSION_MINOR > 1)
    m_about_text.setFillColor(Color::Red);
    #else
    //Only relevant for Travis
    m_about_text.setColor(Color::Red);
    #endif
    m_about_window.draw(m_about_text);
    m_about_window.display();
  }
}
