#include "sfml_button.h"

#include <cassert>

sfml_button::sfml_button(const double x, const double y,
                         const double height, const double width)
  : m_x{x}, m_y{y}, m_height{height}, m_width{width}
{
  m_shape.setSize(sf::Vector2f(m_x,m_y));
}

bool sfml_button::is_clicked(const sf::Event& event,
                             const sf::RenderWindow& window,
                             double camera_x, double camera_y) const {
  double x = sf::Mouse::getPosition(window).x + camera_x;
  double y = sf::Mouse::getPosition(window).y + camera_y;
  if (event.type == sf::Event::MouseButtonPressed) {
    return (x > m_x && x < m_x + m_width &&
            y > m_y && y < m_y + m_height);
  }
  return false;
}
