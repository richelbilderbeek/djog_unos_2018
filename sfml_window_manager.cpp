#include "sfml_window_manager.h"
#include <cassert>

sfml_window_manager *sfml_window_manager::m_instance = nullptr; //!OCLINT static accepted singleton

#if(SFML_VERSION_MINOR > 1)
sfml_window_manager::sfml_window_manager()
  : m_window(sf::VideoMode(
               static_cast<unsigned int>(sf::VideoMode::getDesktopMode().width - 200),
               static_cast<unsigned int>(sf::VideoMode::getDesktopMode().height - 200)),
             "Nature Zen", static_cast<unsigned int>(get_video_mode()))
{
  // Set up window, start location to the center
  m_window.setPosition(sf::Vector2i(100, 50));
  m_screen_center = sf::Vector2i(m_window.getSize().x / 2,
                                 m_window.getSize().y / 2);
}
#else
sfml_window_manager::sfml_window_manager()
  : m_window(sf::VideoMode(
               static_cast<unsigned int>(1000),
               static_cast<unsigned int>(1000)),
             "Nature Zen")
{
  // Set up window, start location to the center
  m_window.setPosition(sf::Vector2i(0, 0));
  m_screen_center = sf::Vector2i(m_window.getSize().x / 2,
                                 m_window.getSize().y / 2);
}
#endif

sfml_window_manager &sfml_window_manager::get() {
  if (!m_instance) {
    m_instance = new sfml_window_manager();
  }
  assert(m_instance);
  return *m_instance;
}

void sfml_window_manager::update_center() {
  m_screen_center = sf::Vector2i(m_window.getView().getSize().x / 2,
                                 m_window.getView().getSize().y / 2);
}

int get_video_mode() {
#ifndef NDEBUG
  return sf::Style::Default;
#else
  return sf::Style::Fullscreen;
#endif
}

/*
void test_sfml_window_manager() //!OCLINT tests may be long
{
  sfml_window_manager &window_manager = sfml_window_manager::get();
}
*/
