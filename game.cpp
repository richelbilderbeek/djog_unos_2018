#include "game.h"

sf::Color shapeColors[] = {sf::Color(255, 255, 255),
                           sf::Color(255, 0, 0),
                           sf::Color(0, 255, 0),
                           sf::Color(0, 0, 255),
                           sf::Color(255, 255, 0),
                           sf::Color(255, 0, 255),
                           sf::Color(0, 255, 255),
                           sf::Color(150, 150, 150)
};

int colorIndex = 0;

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
  shape.setPosition(100 + camera_x, 100 + camera_y);
  shape.setRotation(m_angle);
  shape.setFillColor(shapeColors[colorIndex]);
  m_window.draw(shape);

  // end the current frame
  m_window.display();

}

int game::exec()
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
    // Travis isn't able to test this line which causes -> Codecov 97% (Could be 100%)
    switch(event.type)
    {
        // "close requested" event: we close the window
        case sf::Event::Closed:
            m_window.close();
            break;

        // "key pressed" event: parse events for every key
        case sf::Event::KeyPressed:
            //Check for arrow presses
            //If the Right arrow is pressed move camera to right
            if (event.key.code == sf::Keyboard::Right)
                camera_x += 5;
            //If Left arrow key is pressed move the camrea to the left
            if (event.key.code == sf::Keyboard::Left)
                camera_x -= 5;
            //If Up arrow key is pressed move the camera up
            if (event.key.code == sf::Keyboard::Up)
                camera_y -= 5;
            //If Down arrow key is pressed move the camera down
            if (event.key.code == sf::Keyboard::Down)
                camera_y += 5;
            break;

        // "mouse button pressed" event: parse event vor mousse buttons
        case sf::Event::MouseButtonPressed:
            //Check for mouse buttons
            if (event.mouseButton.button == sf::Mouse::Button::Left)
                colorIndex = (colorIndex + 1) % 7;
            break;

        default:
            break;
      }
  }
}
