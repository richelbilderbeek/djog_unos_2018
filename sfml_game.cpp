#include "sfml_game.h"

sfml_game::sfml_game(int width, int height)
  : m_window(sf::VideoMode(width, height), "Nature Zen")
{
    wheight = height;
    wwidth = width;
}

void sfml_game::display()
{

  // clear the window with black color
  m_window.clear(sf::Color::Black);
  typedef  std::vector <sf::RectangleShape>::iterator tIntIter;
  for( tIntIter iter = shapes.begin(); iter != shapes.end(); iter++)
  {
    m_window.draw(*iter);
  }
  shapes.clear();

  // end the current frame
  m_window.display();

}

int sfml_game::init()
{
  // Set the window start location to the center
  m_window.setPosition(
    sf::Vector2i(
        sf::VideoMode::getDesktopMode().width * 0.5 - wwidth * 0.5,
        sf::VideoMode::getDesktopMode().height * 0.5 - wheight * 0.5
    )
);


  return 0;
}

void sfml_game::add_shape(sf::RectangleShape shape)
{
    sf::Vector2f pos = shape.getPosition();
    shape.setPosition(pos.x + camera_x, pos.y + camera_y);
    shapes.push_back(shape);
}

void sfml_game::move_camera(sf::Vector2f offset){

    camera_x += offset.x;
    camera_y += offset.y;

}

//TODO Create a note with your name below
//Or if you want you can make a "NOTE", "FIXME", "BUG" or "WARNING" instead
