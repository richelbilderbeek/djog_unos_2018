#include "sfml_game.h"

sfml_game::sfml_game()
  : m_n_displayed_max{6000},
    m_window(sf::VideoMode(width, height), "Nature Zen")
{

}

void sfml_game::display()
{
  ++m_n_displayed;

  // clear the window with black color
  m_window.clear(sf::Color::Black);
  typedef  std::vector <sf::RectangleShape>::iterator tIntIter;
  for( tIntIter iter = shapes.begin(); iter != shapes.end(); iter++)
  {
    m_window.draw(*iter);
  }


  // end the current frame
  m_window.display();

}

int sfml_game::exec()
{
  // Set the window start location to the center
  m_window.setPosition(
    sf::Vector2i(
        sf::VideoMode::getDesktopMode().width * 0.5 - width * 0.5,
        sf::VideoMode::getDesktopMode().height * 0.5 - height * 0.5
    )
);

  while (m_window.isOpen())
  {
    process_input();
    process_events();
    display();
  }
  return 0;
}

int extract_n_displayed_max(const std::vector<std::string>& /* args */)
{
  //STUB
  return 1000;
}

void sfml_game::process_events()
{
  m_angle += 0.01;
  if (m_n_displayed_max > 0 && m_n_displayed + 1 == m_n_displayed_max)
  {
    m_window.close();
  }
}

void sfml_game::add_shape(sf::Vector2f size, sf::Vector2f position, float rotation)
{
    sf::RectangleShape shape(size);
    shape.setPosition(position.x + camera_x, position.y + camera_y);
    shape.setRotation(rotation);
    shapes.push_back(shape);
}

void sfml_game::process_input()
{
  // check all the window's events that were triggered since the last iteration of the loop
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    // Pass this
  }
}
