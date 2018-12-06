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
  m_shape.setFillColor(sf::Color(100,100,100));

  m_text.setFont(sfml_resources::get().get_default_font());
  set_string("Click me!");
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
  m_shape.setSize(sf::Vector2f(m_width,m_height));
  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

void sfml_button::set_string(const String str) {
  m_text.setString(str);
  m_text.setOrigin(m_text.getGlobalBounds().left +
                   m_text.getGlobalBounds().width / 2.0f,
                   m_text.getGlobalBounds().top +
                   m_text.getGlobalBounds().height / 2.0f);
  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
}

bool sfml_button::is_clicked(const sf::Event& event,
                             const sf::RenderWindow& window) {
  double x = sf::Mouse::getPosition(window).x;
  double y = sf::Mouse::getPosition(window).y;
  if (event.type == sf::Event::MouseButtonPressed) {
    std::cout << m_text.getOrigin().x << " " << m_shape.getOrigin().y << std::endl;

    return x > m_x && x < m_x + m_width &&
           y > m_y && y < m_y + m_height;
  }
  return false;
}
