#include "sfml_text_input.h"
#include "sfml_resources.h"

#include <iostream>
#include <cassert>

#include <SFML/Main.hpp>
// TODO make color lighter if selected

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
  assert(event.type == sf::Event::MouseButtonPressed);
  m_selected = x > m_x && x < m_x + m_width &&
               y > m_y && y < m_y + m_height;
}

void sfml_text_input::input(const sf::Event& event) {
  assert(event.type == sf::Event::TextEntered);
  if (m_selected) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
      m_selected = false;
    } else if (event.text.unicode == '\b') {
      m_string.resize(m_string.size() - 1);
    } else if (is_normal_char(event.text.unicode)) {
      m_string += static_cast<char>(event.text.unicode);
    }
    set_string(m_string);
  }
}

bool is_normal_char(uint32_t c) {
  return (c > 47 && c < 58) ||
         (c > 64 && c < 91) ||
         (c > 96 && c < 123) ||
         (c == ' ');
}

void test_normal_char() {
  assert(is_normal_char('a'));
  assert(is_normal_char('z'));
  assert(is_normal_char('0'));
  assert(is_normal_char('9'));
  assert(is_normal_char('A'));
  assert(is_normal_char('Z'));
  assert(is_normal_char('G'));
  assert(is_normal_char('g'));
  assert(is_normal_char('1'));
  assert(is_normal_char('5'));
  assert(is_normal_char(' '));

  assert(!is_normal_char('{'));
  assert(!is_normal_char('\\'));
  assert(!is_normal_char('\b'));
  assert(!is_normal_char('\n'));
  assert(!is_normal_char('`'));
  assert(!is_normal_char(';'));
  assert(!is_normal_char('*'));
  assert(!is_normal_char('|'));
}
