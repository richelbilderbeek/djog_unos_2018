#include "sfml_save_overlay.h"
#include "sfml_resources.h"
#include "sfml_button.h"
#include "sfml_window_manager.h"
#include <iostream>
#include <cassert>

sfml_save_overlay::sfml_save_overlay()
    : m_window{ sfml_window_manager::get().get_window() },
      m_font{ sfml_resources::get().get_default_font() }
{
  m_header.setFont(m_font);
  m_header.setCharacterSize(40);
  m_header.setString("Save game");

  sf::RectangleShape &b1_s = m_button1.get_shape();
  b1_s.setFillColor(sf::Color(53,234,151));
  m_button1.set_size(100, 75);
  m_button1.set_string(">");

  sf::RectangleShape &ni_s = m_name_input.get_shape();
  ni_s.setFillColor(sf::Color(53,234,151));
  m_name_input.set_size(300, 75);

  #if(SFML_VERSION_MINOR > 3)
  m_header.setFillColor(sf::Color(51, 51, 51));
  m_header.setOutlineColor(sf::Color(41,180,116));
  m_header.setOutlineThickness(3);

  m_bg_rect.setFillColor(sf::Color(80, 140, 80, 200));
  #else
  //Only relevant for Travis
  m_header.setColor(sf::Color::Green);
  #endif
}

void sfml_save_overlay::exec() //!OCLINT high cyclomatic complexity
{
  assert(active(game_state::saving));
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
      case sf::Event::MouseButtonPressed:
        m_name_input.select(event, m_window);
        break;
//      case sf::Event::KeyPressed:
//        break;
      case sf::Event::TextEntered:
        m_name_input.input(event);
        break;
      default:
        sfml_window_manager::get().process();
        break;
    }
  }
  set_positions();
  draw_objects();
  m_window.display();
}

void sfml_save_overlay::draw_objects() {
  m_window.draw(m_bg_rect);
  m_window.draw(m_header);
  m_window.draw(m_button1.get_shape());
  m_window.draw(m_button1.get_text());
  m_window.draw(m_name_input.get_shape());
  m_window.draw(m_name_input.get_text());
}

void sfml_save_overlay::set_positions() {

  //Button 1
  sf::Vector2f b1_pos(m_window.mapPixelToCoords(sf::Vector2i(
                                                  (m_window.getSize().x /2 + 250),
                                                  (m_window.getSize().y/568)*450)));
  m_button1.set_pos(b1_pos.x, b1_pos.y);

  //Name input
  sf::Vector2f ni_pos(m_window.mapPixelToCoords(sf::Vector2i(
                                                  (m_window.getSize().x / 2),
                                                  (m_window.getSize().y/568)*330)));
  m_name_input.set_pos(ni_pos.x, ni_pos.y);

  m_bg_rect.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 0)));
  m_bg_rect.setSize(m_window.getView().getSize());
}

void sfml_save_overlay::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_save_overlay::close() {
  m_window.close();
}
