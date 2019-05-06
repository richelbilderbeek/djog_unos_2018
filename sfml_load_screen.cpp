#include "sfml_load_screen.h"
#include "sfml_resources.h"
#include "sfml_button.h"
#include "sfml_window_manager.h"
#include "sfml_game.h"
#include "game.h"
#include <iostream>
#include <cassert>

sfml_load_screen::sfml_load_screen()
    : m_window{ sfml_window_manager::get().get_window() },
      m_font{ sfml_resources::get().get_default_font() }
{
  if (get_saves().empty()) {
    close(game_state::playing);
    return;
  }

  m_header.setFont(m_font);
  m_header.setCharacterSize(40);
  m_header.setString("load game");

  sf::RectangleShape &ng_s = m_new_game.get_shape();
  ng_s.setFillColor(sf::Color(53,234,151));
  m_new_game.set_size(590, 75);
  m_new_game.set_string("new game");

  m_list.set_size(500, 200);
  m_list.set_pos(100, 100, m_window);

  // BUG "pure virtual method called" or otherwise not displaying the buttons
  int y = 0;
  sf::Vector2i p = m_window.mapCoordsToPixel(m_list.get_pos());
  sf::View tmp_view = m_window.getView();
  m_window.setView(m_list.get_view());
  for (std::string str : get_saves()) {
    sfml_button b;
    sf::RectangleShape &b_s = b.get_shape();
    b_s.setFillColor(sf::Color(53,234,151));
    b.set_size(100, 75);
    b.set_string(str);
    sf::Vector2f pos = m_window.mapPixelToCoords(sf::Vector2i(p.x + 10, p.y + y));
    b.set_pos(pos.x, pos.y);
    y += 80;
    m_saves.push_back(b);
    m_list.add_rectangle(b.get_shape());
    m_list.add_text(b.get_text());
  }
  m_window.setView(tmp_view);

  #if(SFML_VERSION_MINOR > 3)
  m_header.setFillColor(sf::Color(51, 51, 51));
  m_header.setOutlineColor(sf::Color(41,180,116));
  m_header.setOutlineThickness(3);
  #else
  //Only relevant for Travis
  m_header.setColor(sf::Color::Green);
  #endif
}

void sfml_load_screen::exec()
{
  while(active(game_state::loading))
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
          for (sfml_button b : m_saves) {
            if (b.is_clicked(event, m_window)) {
              sfml_game g;
              g.load_game(b.get_string());
              close(game_state::playing);
              g.exec();
              return;
            }
          }
          if (m_new_game.is_clicked(event, m_window)) {
            close(game_state::playing);
          }
          break;
        default:
          sfml_window_manager::get().process();
          break;
      }
    }
    set_positions();
    draw_objects();
  }
}

void sfml_load_screen::draw_objects() {
  m_window.clear(sf::Color::Black);
  m_window.draw(m_header);
  m_window.draw(m_new_game.get_shape());
  m_window.draw(m_new_game.get_text());
  m_list.draw(m_window);
  m_window.display();
}

void sfml_load_screen::set_positions() {
  //Header
  m_header.setPosition((m_window.getSize().x / 2) - m_header.getGlobalBounds().width/2,
                          m_window.getView().getCenter().y-(m_window.getSize().y/2)+
                          (m_window.getSize().y/568)*80);
  m_header.setPosition(m_window.mapPixelToCoords(
                            sf::Vector2i(m_header.getPosition())));

  sf::Vector2f ng_pos(m_window.mapPixelToCoords(sf::Vector2i(
                                                  (m_window.getSize().x / 2 - 100),
                                                  (m_window.getSize().y/568)*450)));
  m_new_game.set_pos(ng_pos.x, ng_pos.y);

  m_list.set_pos(m_window.getSize().y/2, 200, m_window); // map pixel to coords is built in
}

void sfml_load_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_load_screen::close() {
  m_window.close();
}
