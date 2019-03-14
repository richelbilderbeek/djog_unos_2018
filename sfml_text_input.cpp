#include "sfml_text_input.h"
#include "sfml_resources.h"

#include <iostream>
#include <cassert>

sfml_text_input::sfml_text_input(const double x, const double y,
                         const double height, const double width)
  : m_x{x}, m_y{y}, m_height{height}, m_width{width}
{
  m_shape.setSize(sf::Vector2f(m_width,m_height));
  m_shape.setPosition(sf::Vector2f(m_x,m_y));
  m_shape.setFillColor(sf::Color(100,100,100));

  m_text.setFont(sfml_resources::get().get_default_font());
}

void sfml_text_input::set_pos(double x, double y) {
  m_x = x - (m_width / 2);
  m_y = y - (m_height / 2);
  m_shape.setPosition(m_x, m_y);

  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

void sfml_text_input::set_size(double width, double height) {
  m_width = width;
  m_height = height;
  m_shape.setSize(sf::Vector2f(m_width, m_height));

  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

void sfml_text_input::set_string(const std::string str) {
  m_string = str;
  m_text.setString(str);
  sf::FloatRect bounds = m_text.getLocalBounds();
  m_text.setOrigin(bounds.left + bounds.width/2.0f,
                   bounds.top  + bounds.height/2.0f);
  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

void sfml_text_input::select(const sf::Event& event,
                             const sf::RenderWindow& window) {
  sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
  double x = mouse.x;
  double y = mouse.y;
  if (event.type == sf::Event::MouseButtonPressed) {
    m_selected = x > m_x && x < m_x + m_width &&
                 y > m_y && y < m_y + m_height;
  }
}

void sfml_text_input::input(const sf::Event& event) {
  if (event.type == sf::Event::TextEntered)
  {
    // Handle ASCII characters only
    if (event.text.unicode < 128)
    {
      m_string += static_cast<char>(event.text.unicode);
      set_string(m_string);
    }
  }
}
