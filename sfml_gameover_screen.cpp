#include "sfml_gameover_screen.h"
#include "sfml_resources.h"
#include "sfml_button.h"
#include "sfml_window_manager.h"
#include <iostream>
#include <cassert>

sfml_gameover_screen::sfml_gameover_screen(const int close_at)
    : m_window{ sfml_window_manager::get().get_window() },
      m_font{ sfml_resources::get().get_default_font() },
      m_close_at{close_at}
{
    m_header.setFont(m_font);
    m_header.setString("GAME OVER");

    sf::RectangleShape &b1_s = m_button1.get_shape();
    b1_s.setFillColor(sf::Color::Red);
    m_button1.set_size(325, 100);
    m_button1.set_string("Back to menu");

    m_zen_bar.setTexture(sfml_resources::get().get_zen_bar());
    m_zen_icon.setTexture(sfml_resources::get().get_zen_ind());

    m_zen_bar.setPosition(sf::Vector2f(
                            (m_window.getSize().x/2.0f)-(m_zen_bar.getTextureRect().width/2.0f),
                            15));
    m_zen_bar.setPosition(m_window.mapPixelToCoords(sf::Vector2i(m_zen_bar.getPosition())));
}

void sfml_gameover_screen::exec()
{
  if (m_close_at >= 0) close(game_state::playing);
  while(active(game_state::gameover))
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
            sfml_window_manager::get().set_state(game_state::menuscreen);
          break;
        default:
          sfml_window_manager::get().process();
          break;
      }
    }
    m_window.clear(sf::Color::Black);
    set_positions();
    m_window.draw(m_header);
    m_window.draw(m_button1.get_shape());
    m_window.draw(m_button1.get_text());
    m_window.display();
  }
}

void sfml_gameover_screen::set_positions() {
  //Header
  m_header.setPosition((m_window.getSize().x / 2) - m_header.getGlobalBounds().width/2,
                          m_window.getView().getCenter().y-(m_window.getSize().y/2)+
                          (m_window.getSize().y/568)*110);
  m_header.setPosition(m_window.mapPixelToCoords(
                            sf::Vector2i(m_header.getPosition())));

  //Button 1
  sf::Vector2f b1_pos(m_window.mapPixelToCoords(sf::Vector2i(
                                                  (m_window.getSize().x / 2),
                                                  (m_window.getSize().y/568)*325)));
  m_button1.set_pos(b1_pos.x, b1_pos.y);
}

void sfml_gameover_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_gameover_screen::close() {
  m_window.close();
}
