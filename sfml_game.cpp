//Always include the header of the unit first
#include "sfml_game.h"

#include <cassert>
#include <QFile>

sfml_game::sfml_game(
  const int window_width,
  const int window_height,
  const sfml_game_delegate& delegate
)
  : m_background_music{},
    m_delegate{delegate},
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

sfml_game::~sfml_game()
{
  m_background_music.stop();
}

void sfml_game::add_shape(sf::RectangleShape shape)
{
    sf::Vector2f pos = shape.getPosition();
    shape.setPosition(pos.x + camera_x, pos.y + camera_y);
    shapes.push_back(shape);
}

void sfml_game::close()
{
  m_window.close();
}

void sfml_game::display()
{
  //Clear the window with black color
  m_window.clear(sf::Color::Black);

  for (const tile& t: m_game.get_tiles())
  {
    sf::RectangleShape sfml_tile(
      sf::Vector2f(t.get_width(), t.get_height())
    );
    sfml_tile.setPosition(t.get_x(), t.get_y());
    if (t.get_type() == tile_type::grassland)
    {
      sfml_tile.setFillColor(sf::Color(0, 255, 0));
      sfml_tile.setOutlineThickness(10);
      sfml_tile.setOutlineColor(sf::Color(0, 100, 0));
    }
    else
    {
      assert(!"Display of this tile type not implemented yet");
    }
    m_window.draw(sfml_tile);
  }

  //Put everything on the screen
  m_window.display();
}

void sfml_game::exec()
{
  while (m_window.isOpen())
  {
    process_input();
    process_events();
    display();
  }
}


void sfml_game::move_camera(sf::Vector2f offset)
{
  camera_x += offset.x;
  camera_y += offset.y;
}

void sfml_game::process_events()
{
  m_delegate.do_actions(*this);
  ++m_n_displayed;
}

void sfml_game::process_input()
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

      default:
        //Do nothing by default
        break;
    }
  }
}

void sfml_game::process_keyboard_input(const sf::Event& event)
{
  //Only keyboard events
  assert(event.type == sf::Event::KeyPressed);

  if (event.key.code == sf::Keyboard::Right)
      move_camera(sf::Vector2f(-5, 0));
  if (event.key.code == sf::Keyboard::Left)
      move_camera(sf::Vector2f(5, 0));
  if (event.key.code == sf::Keyboard::Up)
      move_camera(sf::Vector2f(0, 5));
  if (event.key.code == sf::Keyboard::Down)
      move_camera(sf::Vector2f(0, -5));
}

void sfml_game::process_mouse_input(const sf::Event& event)
{
  //Only mouse input
  assert(event.type == sf::Event::MouseButtonPressed);

  if (event.mouseButton.button == sf::Mouse::Left
    && m_background_music.getStatus() != sf::Music::Playing)
  {
    m_background_music.play();
  }
  //TODO need to register when mousebutton is down while hovering over an object (and need that object)
  //if (.getGlobalBounds().contains(sf::Mouse::getPosition().x,sf::Mouse::getPosition().y)) {
  //foreach (shape in sfml_game_object.shapes)
  if (event.mouseButton.button == sf::Mouse::Right
    && m_background_music.getStatus() != sf::Music::Paused
  )
  {
    m_background_music.pause();
  }
}

void sfml_game::stop_music()
{
  m_background_music.stop();
}
