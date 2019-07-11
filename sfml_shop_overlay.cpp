#include "sfml_shop_overlay.h"
#include "sfml_resources.h"
#include "sfml_button.h"
#include "sfml_window_manager.h"
#include "game.h"
#include "sfml_game.h"
#include <iostream>
#include <cassert>
#include <locale>
#include <string>
#include <sstream>
#include <cmath>
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
  b2_s.setFillColor(sf::Color(0,255,0));
  m_button2.set_size(300, 100);
  m_button2.set_string("Grassland | 400");

  sf::RectangleShape &b3_s = m_button3.get_shape();
  b3_s.setFillColor(sf::Color(0,255,0));
  m_button3.set_size(300, 100);
  m_button3.set_string("Beach | 400");

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

void sfml_shop_overlay::exec(game& g, sfml_camera& m_camera)
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
        if (event.key.code == sf::Keyboard::R && follow_tile)
        {
          g.get_tiles().back().rotate_c();
          for(int i = 0; i < 90; i++) g.get_tiles().back().rotate();
        }
        break;
      case sf::Event::MouseButtonPressed:
        if (m_button1.is_clicked(event, m_window)){
          if(follow_tile){
            g.get_tiles().pop_back();
            m_button1.set_string("CLOSE");
            m_header.setString("SHOP");
            follow_tile = false;
          }
          else close(game_state::playing);
        }
        if (m_button2.is_clicked(event, m_window)){
          button_clicked(m_button2.get_text(), g);
        }
        if (m_button3.is_clicked(event, m_window)){
          button_clicked(m_button3.get_text(), g);
        }
        if (follow_tile) place_on_grid(g);
        break;

      default:
        sfml_window_manager::get().process();
        break;
    }
    if(follow_tile){
      double x = sf::Mouse::getPosition(m_window).x + m_camera.x; //50
      double y = sf::Mouse::getPosition(m_window).y + m_camera.y; //106
      switch(g.get_tiles().back().get_rotation()){
        case 0:
          x -= 100;
          y -= 50;
          break;
        case 90:
          x -= 50;
          y -= 106;
          break;
        case 180:
          x += 10;
          y -= 50;
          break;
        case 270:
          x -= 50;
          y += 6;
          break;
      }
      g.get_tiles().back().set_x(x);
      g.get_tiles().back().set_y(y);
    }
  }
  set_positions();
  draw_objects();
  m_window.display();
}

void sfml_shop_overlay::place_on_grid(game& g){
  double current_x = g.get_tiles().back().get_x();
  double current_y = g.get_tiles().back().get_y();
  int placement_x = std::round(current_x / 112);
  int placement_y = std::round(current_y / 112);
  for(int i = 0; i < g.get_tiles().size(); i++){
    if(contains(g.get_tiles()[i], placement_x * 112, placement_y * 112)){
      return;
    }
  }
  g.get_tiles().back().set_x(placement_x * 112);
  g.get_tiles().back().set_y(placement_y * 112);
  if(g.get_essence() > m_price) g.set_essence(g.get_essence() - m_price);
  else{
    m_header.setString("NOT ENOUGH ESSENCE");
    return;
  }
  m_button1.set_string("CLOSE");
  follow_tile = false;
}

void sfml_shop_overlay::button_clicked(sf::Text button, game& g){
  std::cout << "button clicked" << std::endl;
  m_button1.set_string("CANCEL");
  std::stringstream ss(extract_ints(button.getString().toAnsiString()));
  ss >> m_price;
  std::stringstream s(button.getString().toAnsiString());
  std::string type_s;
  s >> type_s;
  std::transform (type_s.begin(), type_s.end(), type_s.begin(), ::tolower);
  tile_type type = to_tile(type_s);
  std::vector<tile> ts;
  tile t(0, 0, 0, 90, 0, type);
  ts.push_back(t);
  g.add_tiles(ts);
  follow_tile = true;
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
  m_window.draw(m_header);
  if (!follow_tile){
    m_window.draw(m_bg_rect);
    m_window.draw(m_button2.get_shape());
    m_window.draw(m_button2.get_text());
    m_window.draw(m_button3.get_shape());
    m_window.draw(m_button3.get_text());
  }
  m_window.draw(m_button1.get_shape());
  m_window.draw(m_button1.get_text());
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
    get_grid_position(m_button2, 0, 0));
  m_button2.set_pos(m_window.mapPixelToCoords(pos1));

  //button 3
  sf::Vector2i pos2 = sf::Vector2i(
    get_grid_position1(m_button3, 0, 0));
  m_button3.set_pos(m_window.mapPixelToCoords(pos2));

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
sf::Vector2f get_grid_position1(sfml_button &b, int x_p, int y_p) {
    return sf::Vector2f(510 + (310 * x_p), 200 + (110 * y_p));

}
