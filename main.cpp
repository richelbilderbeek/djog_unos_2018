#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window(sf::VideoMode(800, 600), "sfml_unos_2018");

  double angle{0.0};

  // run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window.pollEvent(event))
    {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
      window.close();
    }

    // clear the window with black color
    window.clear(sf::Color::Black);

    // draw everything here...
    sf::RectangleShape shape(sf::Vector2f(12.34, 56.78));
    shape.setPosition(100, 100);
    shape.setRotation(angle);
    angle += 0.1;
    window.draw(shape);

    // end the current frame
    window.display();
  }
}
