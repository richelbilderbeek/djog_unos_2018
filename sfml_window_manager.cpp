#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include <cassert>

sfml_window_manager *sfml_window_manager::m_instance = nullptr; //!OCLINT static accepted singleton

#if(SFML_VERSION_MINOR > 3)
sfml_window_manager::sfml_window_manager()
  : m_window(sf::VideoMode(
               static_cast<unsigned int>(sf::VideoMode::getDesktopMode().width - 200),
               static_cast<unsigned int>(sf::VideoMode::getDesktopMode().height - 200)),
             "Nature Zen", static_cast<unsigned int>(get_video_mode())),
    m_state{ game_state::playing }
{
  m_window.setPosition(sf::Vector2i(100, 50));
  m_screen_center = sf::Vector2i(m_window.getSize().x / 2,
                                 m_window.getSize().y / 2);
  m_window.setFramerateLimit(300);
  sf::Image ico = sfml_resources::get().get_zen_ind().copyToImage();
  sf::Vector2u is = ico.getSize();
  m_window.setIcon(is.x, is.y, ico.getPixelsPtr());
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

void sfml_window_manager::update() {
  if (m_window.getSize().y < 568) {
    m_window.setSize(sf::Vector2u(m_window.getSize().x,568));
    m_window.setPosition(m_window_pos);
  }
  if (m_window.getSize().x < 852) {
    m_window.setSize(sf::Vector2u(852,m_window.getSize().y));
    m_window.setPosition(m_window_pos);
  }

  m_screen_center = sf::Vector2i(m_window.getView().getSize().x / 2,
                                 m_window.getView().getSize().y / 2);
}

void sfml_window_manager::process() {
  m_window_pos = m_window.getPosition();
}

void sfml_window_manager::set_state(game_state s) {
  m_state = s;
  sf::Vector2f size = m_window.getView().getSize();
  m_old_view = m_window.getDefaultView();
  m_old_view.setSize(size);
  sfml_window_manager::get().get_window().setView(m_old_view);
}

int get_video_mode() {
#ifndef NDEBUG
  return sf::Style::Default;
#else
  return sf::Style::Fullscreen;
#endif
}

bool active(game_state s) {
  return sfml_window_manager::get().get_state() == s &&
         sfml_window_manager::get().get_window().isOpen();
}

/*
void test_sfml_window_manager() //!OCLINT tests may be long
{
  sfml_window_manager &window_manager = sfml_window_manager::get();
}
*/
