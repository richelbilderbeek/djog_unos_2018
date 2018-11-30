#ifndef SFML_RESOURCES_H
#define SFML_RESOURCES_H

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

  sf::Texture &get_cow_texture() noexcept { return m_cow_texture; }

  sf::Texture &get_fish_texture() noexcept { return m_fish_texture; }

  sf::Texture &get_crocodile_texture() noexcept { return m_crocodile_texture; }

  sf::Texture &get_gras_texture() noexcept { return m_gras_texture; }

  sf::Texture &get_bacterium_texture() noexcept { return m_bacterium_texture; }

  sf::Font& get_default_font() noexcept { return m_default_font; }

  sf::Font& get_title_font() noexcept { return m_title_font; }

  sf::Texture& get_background_image() noexcept { return m_background_image; }

private:
  /// Do not construct a sfml_resources,
  /// use sfml_resources::get() instead
  sfml_resources();

  static sfml_resources *m_instance; //!OCLINT accepted for now

  sf::Font m_default_font;

  sf::Font m_title_font;

  sf::Music m_background_music;

  sf::Music m_title_music;

  sf::Texture m_cow_texture;

  sf::Texture m_fish_texture;

  sf::Texture m_crocodile_texture;

  sf::Texture m_gras_texture;

  sf::Texture m_bacterium_texture;

  sf::Texture m_background_image;
};

/// Test the sfml_resources class
void test_sfml_resources();

#endif // SFML_RESOURCES_H
