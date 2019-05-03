#include "sfml_menu_screen.h"
#include "sfml_resources.h"
#include "sfml_button.h"
#include "sfml_window_manager.h"
#include <iostream>
#include <cassert>

sfml_menu_screen::sfml_menu_screen(const int close_at)
    : m_window{ sfml_window_manager::get().get_window() },
      m_font{ sfml_resources::get().get_default_font() },
      m_close_at{close_at}
{
  m_header.setFont(m_font);
  m_header.setCharacterSize(40);
  m_header.setString("MENU");

  sf::RectangleShape &b1_s = m_button1.get_shape();
  b1_s.setFillColor(sf::Color(53,234,151));
  m_button1.set_size(250, 75);
  m_button1.set_string("PLAY / LOAD");

  sf::RectangleShape &b2_s = m_button2.get_shape();
  b2_s.setFillColor(sf::Color(53,234,151));
  m_button2.set_size(250, 75);
  m_button2.set_string("TEAM OCTANE");

  sf::RectangleShape &b3_s = m_button3.get_shape();
  b3_s.setFillColor(sf::Color(53,234,151));
  m_button3.set_size(250, 75);
  m_button3.set_string("EXIT");

  #if(SFML_VERSION_MINOR > 3)
  m_header.setFillColor(sf::Color(51, 51, 51));
  m_header.setOutlineColor(sf::Color(41,180,116));
  m_header.setOutlineThickness(3);
  #else
  //Only relevant for Travis
  m_header.setColor(sf::Color::Green);
  #endif
}

void sfml_menu_screen::exec()
{
  if (m_close_at >= 0) close(game_state::aboutscreen);
  while(active(game_state::menuscreen))
  {
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      process_event(event);
    }
    set_positions();
    draw_objects();
  }
}

void sfml_menu_screen::process_event(sf::Event event) //!OCLINT indeed too complex
{
  switch (event.type) {
    case sf::Event::Closed:
        close();
        break;
    case sf::Event::Resized:
      {
        sf::View view = m_window.getDefaultView();
        sfml_window_manager::get().update();
        view.setSize(static_cast<float>(m_window.getSize().x),
                     static_cast<float>(m_window.getSize().y));
        m_window.setView(view);
      }
      break;
    case sf::Event::KeyPressed:
      switch (event.key.code)
      {
        case sf::Keyboard::C:
          close(game_state::playing);
          break;
        case sf::Keyboard::T:
          close(game_state::aboutscreen);
          break;
        case sf::Keyboard::Q:
          close();
          break;
        default:
          break;
      }
      break;
    case sf::Event::MouseButtonPressed:
      if (m_button1.is_clicked(event, m_window))
        close(game_state::loading);
      if (m_button2.is_clicked(event, m_window))
        close(game_state::aboutscreen);
      if (m_button3.is_clicked(event, m_window))
        close();
      break;
    default:
      sfml_window_manager::get().process();
      break;
  }
}

void sfml_menu_screen::draw_objects() {
  m_window.clear(sf::Color::Black);
  m_window.draw(m_header);
  m_window.draw(m_button1.get_shape());
  m_window.draw(m_button1.get_text());
  m_window.draw(m_button2.get_shape());
  m_window.draw(m_button2.get_text());
  m_window.draw(m_button3.get_shape());
  m_window.draw(m_button3.get_text());
  m_window.display();
}

void sfml_menu_screen::set_positions() {
  //Header
  m_header.setPosition((m_window.getSize().x / 2) - m_header.getGlobalBounds().width/2,
                          m_window.getView().getCenter().y-(m_window.getSize().y/2)+
                          (m_window.getSize().y/568)*80);
  m_header.setPosition(m_window.mapPixelToCoords(
                            sf::Vector2i(m_header.getPosition())));

  //Button 1
  sf::Vector2f b1_pos(m_window.mapPixelToCoords(sf::Vector2i(
                                                  (m_window.getSize().x / 2),
                                                  (m_window.getSize().y/568)*220)));
  m_button1.set_pos(b1_pos.x, b1_pos.y);

  //Button 2
  sf::Vector2f b2_pos(m_window.mapPixelToCoords(sf::Vector2i(
                                                  (m_window.getSize().x / 2),
                                                  (m_window.getSize().y/568)*330)));
  m_button2.set_pos(b2_pos.x, b2_pos.y);

  //Button 3
  sf::Vector2f b3_pos(m_window.mapPixelToCoords(sf::Vector2i(
                                                  (m_window.getSize().x / 2),
                                                  (m_window.getSize().y/568)*440)));
  m_button3.set_pos(b3_pos.x, b3_pos.y);
}

void sfml_menu_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_menu_screen::close() {
  m_window.close();
}
