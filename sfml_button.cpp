#include "sfml_button.h"
#include "sfml_game.h"

#include <cassert>

sfml_button::sfml_button(const double x = 1.0, const double y = 1.0,
                         const double height = 1.0, const double width = 1.0)
  : m_x{x}, m_y{y}, m_height{height}, m_width{width}
{
  m_shape().setSize(sf::Vector2f(m_x,m_y))
}

bool sfml_button::is_clicked(const sf::Event& event) {
  const double x = sfml_game::get_mouse_pos().x;
  const double y = sfml_game::get_mouse_pos().y;
  if (event.type == sf::Event::MouseButtonPressed) {
    return (x > m_x && x < m_x + m_width &&
            y > m_y && y < m_y + m_height);
  }
}
