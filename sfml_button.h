#ifndef SFML_BUTTON_H
#define SFML_BUTTON_H

#include <SFML/Graphics.hpp>

class sfml_button
{
public:

  /// @param The x coordinate of the button
  /// @param The y coordinate of the button
  /// @param The width of the button
  /// @param The heigth of the button
  sfml_button(const double x = 1.0, const double y = 1.0,
              const double height = 100.0, const double width = 100.0);

  /// Is the button clicked?
  bool is_clicked(const sf::Event& event, const sf::RenderWindow& window);

  sf::RectangleShape &get_shape() noexcept { return m_shape; }
  sf::Text &get_text() noexcept { return m_text; }

  sf::Vector2f get_pos() noexcept { return sf::Vector2f(m_x, m_y); }
  sf::Vector2f get_size() noexcept { return sf::Vector2f(m_width, m_height); }

  void set_string(const std::string& str);
  const std::string& get_string() const noexcept { return m_string; }

  void set_pos(double x, double y);
  void set_pos(sf::Vector2f position);

  void set_size(double width, double height);

private:

  sf::RectangleShape m_shape;
  sf::Text m_text;

  double m_x;
  double m_y;

  double m_height;
  double m_width;

  std::string m_string;

};

#endif // SFML_BUTTON_H
