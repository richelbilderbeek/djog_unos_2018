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

  sf::Music &get_background_music() noexcept { return m_background_music; }

  sf::Music &get_title_music() noexcept { return m_title_music; }

  sf::Music &get_benikeenspin() noexcept { return m_benikeenspin; }

  sf::Font& get_default_font() noexcept { return m_default_font; }

  sf::Font& get_title_font() noexcept { return m_title_font; }

  sf::Texture &get_background_image() noexcept { return m_background_image; }

  sf::Texture &get_agent_sprite(const agent& a) noexcept;

  sf::Texture &get_zen_bar() noexcept { return m_zen_bar_texture; }

  sf::Texture &get_zen_ind() noexcept { return m_zen_ind_texture; }

private:
  /// Do not construct a sfml_resources,
  /// use sfml_resources::get() instead
  sfml_resources();

  static sfml_resources *m_instance; //!OCLINT accepted for now

  sf::Font m_default_font;

  sf::Font m_title_font;

  sf::Music m_background_music;

  sf::Music m_title_music;

  sf::Music m_benikeenspin;

  sf::Texture m_cow_texture;

  sf::Texture m_fish_texture;

  sf::Texture m_crocodile_texture;

  sf::Texture m_tree_texture;

  sf::Texture m_grass_texture;

  sf::Texture m_goat_texture;

  sf::Texture m_none_texture;

  sf::Texture m_spider_texture;

  sf::Texture m_bacterium_texture;

  sf::Texture m_background_image;

  sf::Texture m_bird_texture;

  sf::Texture m_zen_ind_texture;

  sf::Texture m_zen_bar_texture;

};

/// Test the sfml_resources class
void test_sfml_resources();

#endif // SFML_RESOURCES_H
