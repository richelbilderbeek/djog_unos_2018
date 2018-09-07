#include <SFML/Graphics/RectangleShape.hpp>

int main()
{
  ::sf::RectangleShape shape(::sf::Vector2f(100.0,250.0));
  if (shape.getSize().x < 000'050) return 1;
}
