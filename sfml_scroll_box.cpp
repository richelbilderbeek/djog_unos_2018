#include "sfml_scroll_box.h"

sfml_scroll_box::sfml_scroll_box(const double x, const double y,
                                 const double height, const double width)
  : m_x{x}, m_y{y}, m_height{height}, m_width{width}
{
  m_shape.setSize(sf::Vector2f(m_width,m_height));
  m_shape.setPosition(sf::Vector2f(m_x,m_y));

  m_shape.setFillColor(sf::Color(53,234,151,0));
  m_shape.setOutlineThickness(5);
  m_shape.setOutlineColor(sf::Color(43,189,121));

  m_view = sf::View(sf::Vector2f(65, 55),
                    sf::Vector2f(230, 190));
}

void sfml_scroll_box::set_pos(int x, int y, sf::RenderWindow &window) {
  m_x = x;
  m_y = y;
  m_shape.setPosition(window.mapPixelToCoords(sf::Vector2i(m_x, m_y)));

  float x_pos = m_shape.getPosition().x / window.getSize().x;
  float y_pos = m_shape.getPosition().y / window.getSize().y;
  float x_per = m_shape.getSize().x / window.getSize().x;
  float y_per = m_shape.getSize().y / window.getSize().y;
  m_view.setViewport(sf::FloatRect(x_pos, y_pos, x_per, y_per));
  //m_view.setCenter(0, 0);
}

bool sfml_scroll_box::is_clicked(sf::Vector2f pos) const {
  double x = pos.x;
  double y = pos.y;
  return x > m_x && x < m_x + m_width &&
         y > m_y && y < m_y + m_height;
}


void sfml_scroll_box::set_size(double width, double height) {
  m_width = width;
  m_height = height;
  m_shape.setSize(sf::Vector2f(m_width, m_height));
}

void sfml_scroll_box::draw(sf::RenderWindow& window) {
  const sf::View tmp_view = window.getView();
  window.draw(m_shape);
  window.setView(m_view);
  for (sf::RectangleShape& r : m_rectangles) {
    window.draw(r);
  }
  for (sf::Text& t : m_texts) {
    window.draw(t);
  }
  window.setView(tmp_view);
}

void sfml_scroll_box::draw(sf::RenderWindow& window, std::vector<sfml_button> v) {
  const sf::View tmp_view = window.getView();
  window.draw(m_shape);
  window.setView(m_view);
  for (sfml_button &b : v) {
    window.draw(b.get_shape());
    window.draw(b.get_text());
  }
  window.setView(tmp_view);
}

void sfml_scroll_box::add_rectangle(sf::RectangleShape& r) {
  m_rectangles.push_back(std::ref(r));
}

void sfml_scroll_box::add_text(sf::Text& t) {
  m_texts.push_back(std::ref(t));
}

void sfml_scroll_box::scroll(sf::Event&
  #if(SFML_VERSION_MINOR > 3)
  event
  #endif
) {

  #if(SFML_VERSION_MINOR > 3)
  if (event.type != sf::Event::MouseWheelScrolled) return;
  m_view.move(0, event.mouseWheelScroll.delta);
  #endif
  //m_view.setViewport(m_shape.getGlobalBounds());
}
