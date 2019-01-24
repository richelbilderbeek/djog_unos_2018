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
      m_font{ sfml_resources::get().get_default_font() },
      m_zen_font{ sfml_resources::get().get_title_font() }
{
  m_header.setFont(m_zen_font);
  m_header.setCharacterSize(36); // in pixels, not points!
  // set the color
  #if(SFML_VERSION_MINOR > 3)
  m_header.setFillColor(sf::Color::Magenta);
  m_header.setOutlineColor(sf::Color::Green);
  m_header.setOutlineThickness(3);
  #else
  //Only relevant for Travis
  m_header.setColor(sf::Color::Green);
  #endif
  m_header.setString("TEAM OCTANE");
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
          break;
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

    m_header.setPosition(25, 25);

    m_header.setPosition(m_window.mapPixelToCoords(
                         sf::Vector2i(m_header.getPosition())));

    m_window.draw(m_header);

    m_window.display();
  }
}
