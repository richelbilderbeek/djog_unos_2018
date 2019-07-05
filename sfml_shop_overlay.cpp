#include "sfml_shop_overlay.h"
#include "sfml_resources.h"
#include "sfml_button.h"
#include "sfml_window_manager.h"
#include "game.h"
#include <iostream>
#include <cassert>
#include <locale>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cstring>

sfml_shop_overlay::sfml_shop_overlay()
    : m_window{ sfml_window_manager::get().get_window() },
      m_font{ sfml_resources::get().get_default_font() }
{
  m_header.setFont(m_font);
  m_header.setCharacterSize(40);
  m_header.setString("SHOP");

  sf::RectangleShape &b1_s = m_button1.get_shape();
  b1_s.setFillColor(sf::Color(53,234,151));
  m_button1.set_size(100, 100);
  m_button1.set_string("CLOSE");

  sf::RectangleShape &b2_s = m_button2.get_shape();
  b2_s.setFillColor(sf::Color(53,234,151));
  m_button2.set_size(300, 100);
  m_button2.set_string("grassland | 400");

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

void sfml_shop_overlay::exec(game& g)
{
  assert(active(game_state::shop));
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
        if (event.key.code == sf::Keyboard::Escape)
        {
          close(game_state::playing);
        }
        break;
      case sf::Event::MouseButtonPressed:
        if (m_button1.is_clicked(event, m_window))
          close(game_state::playing);
        if (m_button2.is_clicked(event, m_window)){
          button_clicked(m_button2.get_text(), g);
        }
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

void sfml_shop_overlay::button_clicked(sf::Text button, game& g){
  std::cout << "button clicked" << std::endl;
  int price;
  std::stringstream ss(extract_ints(button.getString().toAnsiString()));
  ss >> price;
  if(g.get_essence() > price) g.set_essence(g.get_essence() - price);
  else return;
  std::stringstream s(button.getString().toAnsiString());
  std::string type_s;
  s >> type_s;
  tile_type type = to_tile(type_s);
  std::vector<tile> ts;
  tile t(0, 0, 0, 0, 0, type);
  ts.push_back(t);
  g.add_tiles(ts);
//  bool clicked = false;
//  while(!clicked){
//    sf::Vector2f mouse = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
//    g.get_tiles().back().set_x(mouse.x);
//    g.get_tiles().back().set_y(mouse.y);
//  }
}

std::string sfml_shop_overlay::extract_ints(std::ctype_base::mask category, std::string str, std::ctype<char> const& facet)
{
    using std::strlen;

    char const *begin = &str.front(),
               *end   = &str.back();

    auto res = facet.scan_is(category, begin, end);

    begin = &res[0];
    end   = &res[strlen(res)];

    return std::string(begin, end);
}

std::string sfml_shop_overlay::extract_ints(std::string str)
{
    return extract_ints(std::ctype_base::digit, str,
         std::use_facet<std::ctype<char>>(std::locale("")));
}

void sfml_shop_overlay::draw_objects() {
  m_window.draw(m_bg_rect);
  m_window.draw(m_header);
  m_window.draw(m_button1.get_shape());
  m_window.draw(m_button1.get_text());
  m_window.draw(m_button2.get_shape());
  m_window.draw(m_button2.get_text());
}

void sfml_shop_overlay::set_positions() {
  //Header
  m_header.setPosition((m_window.getSize().x / 2) - m_header.getGlobalBounds().width/2,
                          m_window.getView().getCenter().y-(m_window.getSize().y/2)+
                          (m_window.getSize().y/568)*80);
  m_header.setPosition(m_window.mapPixelToCoords(
                            sf::Vector2i(m_header.getPosition())));

  // BG
  m_bg_rect.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 0)));
  m_bg_rect.setSize(m_window.getView().getSize());

  // Button 1
  sf::Vector2i pos = sf::Vector2i(
    m_window.getSize().x - (m_button1.get_size().x / 2),
    m_window.getSize().y - (m_button1.get_size().y / 2)
  );
  m_button1.set_pos(m_window.mapPixelToCoords(pos));

  // Button 2
  sf::Vector2i pos1 = sf::Vector2i(
    get_grid_position(m_button2, 0, 0)
  );
  m_button2.set_pos(m_window.mapPixelToCoords(pos1));
}

void sfml_shop_overlay::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_shop_overlay::close() {
  m_window.close();
}

sf::Vector2f get_grid_position(sfml_button &b, int x_p, int y_p) {
    return sf::Vector2f(200 + (310 * x_p), 200 + (110 * y_p));
}
