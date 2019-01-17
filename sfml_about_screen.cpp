#include "sfml_about_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
//#include "Text.hpp"
//#include <SFML/Graphics.hpp>
#include <QFile>
#include <cassert>

sfml_about_screen::sfml_about_screen(const int close_at)
    : m_window{ sfml_window_manager::get().get_window() },
      m_close_at{ close_at },
      m_font{ sfml_resources::get().get_default_font() }
{
  m_text.setFont(m_font);
  m_text.setCharacterSize(24); // in pixels, not points!
  // set the color
  #if(SFML_VERSION_MINOR > 1)
  m_text.setFillColor(sf::Color::Green);
  #else
  //Only relevant for Travis
  m_text.setColor(sf::Color::Green);
  #endif
  m_text.setString("Team Octane");
}

void sfml_about_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_about_screen::close()
{
  m_window.close();
}

void sfml_about_screen::exec()
{
  if (m_close_at >= 0) close(game_state::playing);
  while (active(game_state::aboutscreen))
  {
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      sf::View view = m_window.getDefaultView();
      switch (event.type) //!OCLINT too few branches, please fix
      {
        case sf::Event::Closed:
          close();
          break;
        case sf::Event::Resized:
          sfml_window_manager::get().update();
          view.setSize(static_cast<float>(m_window.getSize().x),
                       static_cast<float>(m_window.getSize().y));
          m_window.setView(view);
        case sf::Event::KeyPressed:
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            close(game_state::menuscreen);
          break;
        default:
          sfml_window_manager::get().process();
          break;
      }
    }

    m_window.clear(sf::Color::Black); // Clear the window with black color

    m_text.setPosition(m_window.mapPixelToCoords(
                         sf::Vector2i(m_text.getPosition())));

    m_window.draw(m_text);

    m_window.display();
  }
}
