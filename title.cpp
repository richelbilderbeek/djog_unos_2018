#include "title.h"

#include <cassert>
#include <QFile>

title::title(
  const int window_width,
  const int window_height
)
  : m_background_music{},
    m_window(sf::VideoMode(window_width, window_height), "Nature Zen")
{

  //Resources
  {
    //Re-create resource at executable's location
    { QFile f(":/nature_zen/resources/background_music.ogg"); f.copy("background_music.ogg"); }
  }
  //Set up music
  {
    m_background_music.setLoop(true);
    if (!m_background_music.openFromFile("background_music.ogg"))
    {
      throw std::runtime_error("Cannot find music file 'background_music.ogg'");
    }
    m_background_music.play();
  }
  //Set up window, start location to the center
  m_window.setPosition(
    sf::Vector2i(
        sf::VideoMode::getDesktopMode().width * 0.5 - window_width * 0.5,
        sf::VideoMode::getDesktopMode().height * 0.5 - window_height * 0.5
    )
  );
}

title::~title()
{
  m_background_music.stop();
}

void title::close()
{
  m_window.close();
}

void title::display()
{
  //Clear the window with black color
  m_window.clear(sf::Color::Blue);
  {

  }
  //Put everything on the screen
  m_window.display();
}

void title::exec()
{
  while (m_window.isOpen())
  {
    process_input();
    process_events();
    display();
  }
}

void title::process_events()
{
  ++m_n_displayed;
}

void title::process_input()
{
  // check all the window's events that were triggered since the last iteration of the loop
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    switch (event.type) {
      case sf::Event::Closed:
        close();
        break;

      case sf::Event::KeyPressed:
        process_keyboard_input(event);
        break;

      case sf::Event::MouseButtonPressed:
        process_mouse_input(event);
        break;

      case sf::Event::KeyReleased:
        process_keyboard_input(event);
        break;

      default:
        //Do nothing by default
        break;
    }
  }
}

void title::process_keyboard_input(const sf::Event& event)
{
  //Only keyboard events
  assert(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased);


}

void title::process_mouse_input(const sf::Event& event)
{
  //Only mouse input
  assert(event.type == sf::Event::MouseButtonPressed);

}

void title::stop_music()
{
  m_background_music.stop();
}
