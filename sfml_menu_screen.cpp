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
    m_main_text.setFont(m_font);
    m_main_text.setString("MAIN MENU");
    m_main_text.setOrigin(m_main_text.getGlobalBounds().left
        + m_main_text.getGlobalBounds().width / 2.0f,
      m_main_text.getGlobalBounds().top
        + m_main_text.getGlobalBounds().height / 2.0f);

    sf::RectangleShape &b1_s = m_button1.get_shape();
    b1_s.setFillColor(sf::Color::Yellow);
    m_button1.set_size(250, 75);
    m_button1.set_string("Continue");
}

void sfml_menu_screen::exec()
{
  if (m_close_at >= 0) close(game_state::aboutscreen);
  while(active(game_state::menuscreen))
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
        case sf::Event::MouseButtonPressed:
          if (m_button1.is_clicked(event, m_window))
            sfml_window_manager::get().set_state(game_state::playing);
          break;
        default:
          sfml_window_manager::get().process();
          break;
      }
    }
    m_window.clear(sf::Color::Cyan);
    m_window.draw(m_main_text);
    m_window.draw(m_button1.get_shape());
    m_window.draw(m_button1.get_text());
    m_window.display();
  }
}

void sfml_menu_screen::set_positions() {
  //Header
  m_main_text.setPosition(m_window.getSize().x/2,
                          m_window.getView().getCenter().y-(m_window.getSize().y/2)+
                          (m_window.getSize().y/568)*110);
  m_main_text.setPosition(m_window.mapPixelToCoords(
                            sf::Vector2i(m_main_text.getPosition())));

  //Button 1
  m_button1.set_pos(m_window.getSize().x/2,
                    m_window.getView().getCenter().y-(m_window.getSize().y/2)+
                    (m_window.getSize().y/568)*250);
  m_button1.get_shape().setPosition(m_window.mapPixelToCoords(
                                      sf::Vector2i(m_button1.get_shape().getPosition())));
  m_button1.get_text().setPosition(m_window.mapPixelToCoords(
                                      sf::Vector2i(m_button1.get_text().getPosition())));

}

void sfml_menu_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_menu_screen::close() {
  m_window.close();
}
