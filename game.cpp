#include "game.h"

game::game()
  : m_angle{0.0},
    m_window(sf::VideoMode(800, 600), "sfml_unos_2018")
{

}

void game::display()
{
  // clear the window with black color
  m_window.clear(sf::Color::Black);

  // draw everything here...
  sf::RectangleShape shape(sf::Vector2f(12.34, 56.78));
  shape.setPosition(100, 100);
  shape.setRotation(m_angle);
  m_window.draw(shape);

  // end the current frame
  m_window.display();

}

int game::exec()
{
  while (m_window.isOpen())
  {
    process_input();
    process_events();
    display();
  }
  return 0;
}

void game::process_events()
{
  m_angle += 0.01;
}

void game::process_input()
{
  // check all the window's events that were triggered since the last iteration of the loop
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
    m_window.close();
  }
}
