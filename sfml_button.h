#ifndef SFML_BUTTON_H
#define SFML_BUTTON_H

#include <SFML/Graphics.hpp>

using namespace sf;

class sfml_button
{
public:

  /// @param The x coordinate of the button
  /// @param The y coordinate of the button
  /// @param The width of the button
  /// @param The heigth of the button
  sfml_button(const double x = 1.0, const double y = 1.0,
              const double height = 1.0, const double width = 1.0);

  /// Is the button clicked?
  bool is_clicked(const sf::Event& event, const sf::RenderWindow& window,
                  double camera_x = 0.0, double camera_y = 0.0) const;

  sf::RectangleShape &get_shape() noexcept { return m_shape; }

private:

  sf::RectangleShape m_shape;

  double m_x;
  double m_y;

  double m_height;
  double m_width;

};

#endif // SFML_BUTTON_H
