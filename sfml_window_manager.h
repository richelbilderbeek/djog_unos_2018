#ifndef SFML_WINDOW_MANAGER_H
#define SFML_WINDOW_MANAGER_H

#include "game_state.h"
#include <SFML/Graphics.hpp>

class sfml_window_manager
{
public:

  static sfml_window_manager &get();

  sf::RenderWindow &get_window() { return m_window; }

  sf::Vector2i get_window_center() noexcept { return m_screen_center; }

  void close() { m_window.close(); }

  void update();

  game_state get_state() const noexcept { return m_state; }

  void set_state(game_state s);

  void process();

private:

  sfml_window_manager();

  static sfml_window_manager *m_instance; //!OCLINT accepted for now

  sf::RenderWindow m_window;

  sf::Vector2i m_screen_center;

  sf::Vector2i m_window_pos;

  game_state m_state;

  sf::View m_old_view;

};

bool active(game_state s);

//void test_sfml_window_manager();

///Get the video mode
int get_video_mode();

#endif // SFML_WINDOW_MANAGER_H
