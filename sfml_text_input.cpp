#include "sfml_text_input.h"
#include "sfml_resources.h"

#include <iostream>
#include <cassert>

sfml_text_input::sfml_text_input(const double x, const double y,
                         const double height, const double width)
  : m_x{x}, m_y{y}, m_height{height}, m_width{width},
    m_selected{false}, m_str_size{0}, m_limit{20}
{
  m_shape.setSize(sf::Vector2f(m_width,m_height));
  m_shape.setPosition(sf::Vector2f(m_x,m_y));
  m_shape.setFillColor(sf::Color(53,234,151));

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
  if (m_str_size == 0 && m_string.size() == 1) {
    m_text.setOrigin(bounds.left + bounds.width/2.0f,
                     bounds.top  + bounds.height/2.0f);
  } else {
    m_text.setOrigin(bounds.left + bounds.width/2.0f,
                     m_text.getOrigin().y);
  }
  m_text.setPosition(m_x + (m_width / 2), m_y + (m_height / 2));
  m_str_size = m_string.size();
}

void sfml_text_input::select(const sf::RenderWindow& window) {
  sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
  double x = mouse.x;
  double y = mouse.y;
//  assert(event.type == sf::Event::MouseButtonPressed);
  m_selected = x > m_x && x < m_x + m_width &&
               y > m_y && y < m_y + m_height;
}

void sfml_text_input::input(const sf::Event& event) {
  assert(event.type == sf::Event::TextEntered);
  if (m_selected) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
      m_selected = false;
    } else if (event.text.unicode == '\b' && m_string.size() > 0) {
      m_string.resize(m_string.size() - 1);
    } else if (is_normal_char(event.text.unicode) &&
               static_cast<int>(m_string.size()) < m_limit) {
      m_string += static_cast<char>(event.text.unicode);
    }
    set_string(m_string);
  }
}

sf::RectangleShape &sfml_text_input::get_shape() {
  return m_shape;
}

void sfml_text_input::update() {
  if (m_selected) {
    m_shape.setFillColor(m_select_color);
  } else {
    m_shape.setFillColor(m_color);
  }
}

void sfml_text_input::set_color(sf::Color c) {
  m_color = c;
//  float high = 1.30f;
  float low = 0.70f;
//  if ((c.r * high > 255 && c.g * high > 255) ||
//      (c.g * high > 255 && c.b * high > 255) ||
//      (c.b * high > 255 && c.g * high > 255)) {
    if (c.r * low < 0) c.r = 0;
    if (c.g * low < 0) c.g = 0;
    if (c.b * low < 0) c.b = 0;
    m_select_color = sf::Color(c.r * low, c.g * low, c.b * low, c.a);
//  } else {
//    if (c.r * high > 255) c.r = 255/high;
//    if (c.g * high > 255) c.g = 255/high;
//    if (c.b * high > 255) c.b = 255/high;
//    m_select_color = sf::Color(c.r * high, c.g * high, c.b * high, c.a);
//  }
}

bool is_normal_char(uint32_t c) {
  return (c > 47 && c < 58) ||
         (c > 64 && c < 91) ||
         (c > 96 && c < 123) ||
         (c == ' ');
}

void test_normal_char() { //!OCLINT tests can be complex
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
