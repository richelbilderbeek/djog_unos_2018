#ifndef SFML_RESOURCES_H
#define SFML_RESOURCES_H

#include "agent.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

/// Singleton class to access all resources
//TODO: make this a member variable of sfml_game
class sfml_resources {
public:
  /// Access the resources
  static sfml_resources &get();

  Music &get_background_music() noexcept { return m_background_music; }

  Music &get_title_music() noexcept { return m_title_music; }

  Music &get_benikeenspin() noexcept { return m_benikeenspin; }

  Font& get_default_font() noexcept { return m_default_font; }

  Font& get_title_font() noexcept { return m_title_font; }

  Texture &get_background_image() noexcept { return m_background_image; }

  Texture &get_agent_sprite(const agent& a) noexcept;

  Texture &get_zen_bar() noexcept { return m_zen_bar_texture; }

  Texture &get_zen_ind() noexcept { return m_zen_ind_texture; }

private:
  /// Do not construct a sfml_resources,
  /// use sfml_resources::get() instead
  sfml_resources();

  static sfml_resources *m_instance; //!OCLINT accepted for now

  Font m_default_font;

  Font m_title_font;

  Music m_background_music;

  Music m_title_music;

  Music m_benikeenspin;

  Texture m_cow_texture;

  Texture m_fish_texture;

  Texture m_crocodile_texture;

  Texture m_tree_texture;

  Texture m_grass_texture;

  Texture m_none_texture;

  Texture m_spider_texture;

  Texture m_bacterium_texture;

  Texture m_background_image;

  Texture m_bird_texture;

  Texture m_zen_ind_texture;

  Texture m_zen_bar_texture;

};

/// Test the sfml_resources class
void test_sfml_resources();

#endif // SFML_RESOURCES_H
