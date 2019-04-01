#include "sfml_scroll_box.h"
#include "sfml_button.h"

sfml_scroll_box::sfml_scroll_box(const double x, const double y,
                                 const double height, const double width)
  : m_x{x}, m_y{y}, m_height{height}, m_width{width}
{
  // NOTE only works with buttons
  m_shape.setSize(sf::Vector2f(m_width,m_height));
  m_shape.setPosition(sf::Vector2f(m_x,m_y));

  m_shape.setFillColor(sf::Color(53,234,151,255));
  m_shape.setOutlineThickness(5);
  m_shape.setOutlineColor(sf::Color(43,189,121));

  m_view.setViewport(m_shape.getGlobalBounds());
}

void sfml_scroll_box::set_pos(double x, double y) {
  m_x = x - (m_width / 2);
  m_y = y - (m_height / 2);
  m_shape.setPosition(m_x, m_y);

  m_view.setViewport(m_shape.getGlobalBounds());
}

void sfml_scroll_box::set_size(double width, double height) {
  m_width = width;
  m_height = height;
  m_shape.setSize(sf::Vector2f(m_width, m_height));

  m_view.setSize(m_shape.getSize());
  m_view.setViewport(m_shape.getGlobalBounds());
}

void sfml_scroll_box::draw(sf::RenderWindow& window) {
  const sf::View tmp_view = window.getView();
  window.setView(m_view);
  window.draw(m_shape);
  for (sfml_button &d : m_drawables) {
    window.draw(d.get_shape());
    window.draw(d.get_text());
  }
  window.setView(tmp_view);
}

void sfml_scroll_box::add_drawable(sfml_button &drawable) {
  m_drawables.push_back(std::ref(drawable));
}

void sfml_scroll_box::remove_drawable(sfml_button &drawable) {
  drawable = m_drawables.back();
  m_drawables.pop_back();
}

void sfml_scroll_box::scroll(sf::Event &event) {
  if (event.type != sf::Event::MouseWheelScrolled) return;
  m_view.move(0, event.mouseWheelScroll.delta);
  m_view.setViewport(m_shape.getGlobalBounds());
}
