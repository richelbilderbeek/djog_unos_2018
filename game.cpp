#include "game.h"

game::game(const std::vector<std::string>& args)
  : m_n_displayed_max{extract_n_displayed_max(args)},
    m_window(sf::VideoMode(width, height), "sfml_unos_2018")
{

}

void game::display()
{
  ++m_n_displayed;

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
  return 100;
}

void game::process_events()
{
  m_angle += 0.01;
  if (m_n_displayed_max > 0 && m_n_displayed + 1 == m_n_displayed_max)
  {
    m_window.close();
  }
}

void game::process_input()
{
  // check all the window's events that were triggered since the last iteration of the loop
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
    {
      m_window.close();
    }
  }
}
