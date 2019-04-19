#include "sfml_button.h"
#include "sfml_resources.h"

#include <iostream>
#include <cassert>

sfml_button::sfml_button(const double x, const double y,
                         const double height, const double width)
  : m_x{x}, m_y{y}, m_height{height}, m_width{width}
{
  m_shape.setSize(sf::Vector2f(m_width,m_height));
  m_shape.setPosition(sf::Vector2f(m_x,m_y));
  m_shape.setFillColor(sf::Color(53,234,151));

  m_text.setFont(sfml_resources::get().get_default_font());
  set_string("Click me!");
}

void sfml_button::set_pos(sf::Vector2f position)
{
  m_x = position.x - (m_width / 2);
  m_y = position.y - (m_height / 2);
  m_shape.setPosition(m_x, m_y);

  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

void sfml_button::set_pos(double x, double y) {
  m_x = x - (m_width / 2);
  m_y = y - (m_height / 2);
  m_shape.setPosition(m_x, m_y);

  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

void sfml_button::set_size(double width, double height) {
  m_width = width;
  m_height = height;
  m_shape.setSize(sf::Vector2f(m_width, m_height));

  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

void sfml_button::set_string(const std::string& str) {
  m_string = str;
  m_text.setString(str);
  sf::FloatRect bounds = m_text.getLocalBounds();
  m_text.setOrigin(bounds.left + bounds.width/2.0f,
                   bounds.top  + bounds.height/2.0f);
  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

bool sfml_button::is_clicked(const sf::Event& event,
                             const sf::RenderWindow& window) {
  sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
  double x = mouse.x;
  double y = mouse.y;
  if (event.type == sf::Event::MouseButtonPressed) {
    return x > m_x && x < m_x + m_width &&
           y > m_y && y < m_y + m_height;
  }
  return false;
}
