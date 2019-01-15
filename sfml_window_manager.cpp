#include "sfml_window_manager.h"
#include <cassert>

sfml_window_manager *sfml_window_manager::m_instance = nullptr; //!OCLINT static accepted singleton

sfml_window_manager::sfml_window_manager()
  : m_window(sf::VideoMode(
               static_cast<unsigned int>(sf::VideoMode::getDesktopMode().width),
               static_cast<unsigned int>(sf::VideoMode::getDesktopMode().height)),
             "Nature Zen", static_cast<unsigned int>(get_video_mode()))
{
  // Set up window, start location to the center
  m_window.setPosition(sf::Vector2i(0, 0));
  m_screen_center = sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2,
                                 sf::VideoMode::getDesktopMode().height / 2);
}

sfml_window_manager &sfml_window_manager::get() {
  if (!m_instance) {
    m_instance = new sfml_window_manager();
  }
  assert(m_instance);
  return *m_instance;
}

int get_video_mode()
{
  int s = sf::Style::Fullscreen; //!OCLINT local variable redundant
//  if (std::getenv("TRAVIS"))
//  {
//    s = Style::Default;
//  }
  return s;
}

/*
void test_sfml_window_manager() //!OCLINT tests may be long
{
  sfml_window_manager &window_manager = sfml_window_manager::get();
}
*/
