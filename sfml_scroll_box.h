﻿#ifndef SFML_SCROLL_BOX_H
#define SFML_SCROLL_BOX_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <sfml_button.h>

class sfml_scroll_box
{
public:

  /// @param The x coordinate of the scroll_box
  /// @param The y coordinate of the scroll_box
  /// @param The width of the scroll_box
  /// @param The heigth of the scroll_box
  sfml_scroll_box(const double x = 1.0, const double y = 1.0,
                  const double height = 100.0, const double width = 100.0);

  /// Is the scroll_box hovered above
  bool hovering(const sf::Event& event, const sf::RenderWindow& window);

  sf::RectangleShape &get_shape() noexcept { return m_shape; }

  sf::View &get_view() noexcept { return m_view; }

  sf::Vector2f get_pos() noexcept { return sf::Vector2f(m_x, m_y); }
  sf::Vector2f get_size() noexcept { return sf::Vector2f(m_width, m_height); }

  void set_pos(int x, int y, sf::RenderWindow &window);
  void set_size(double width, double height);

  void draw(sf::RenderWindow& window);
  void draw(sf::RenderWindow& window, std::vector<sfml_button> v);

  void add_rectangle(sf::RectangleShape &r);
  void add_text(sf::Text &t);

  void scroll(sf::Event &event);

  bool is_clicked(sf::Vector2f pos) const;

  int get_scroll() const noexcept { return m_scroll; }

  void set_scroll(int scr) { m_scroll = scr; }

private:

  sf::RectangleShape m_shape;
  sf::View m_view;

  double m_x;
  double m_y;

  double m_height;
  double m_width;

  //No, we are not going to use abstract base classes in this project
  //std::vector<std::reference_wrapper<sf::Drawable>> m_drawables;
  std::vector<std::reference_wrapper<sf::RectangleShape>> m_rectangles;

  std::vector<std::reference_wrapper<sf::Text>> m_texts;

  int m_scroll;

};

#endif // SFML_SCROLL_BOX_H
