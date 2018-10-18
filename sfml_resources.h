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

  sf::Texture &get_cow_texture() noexcept { return m_cow_texture; }

  sf::Font& get_default_font() noexcept { return m_default_font; }

private:
  /// Do not construct a sfml_resources,
  /// use sfml_resources::get() instead
  sfml_resources();

  static sfml_resources *m_instance; //!OCLINT accepted for now

  sf::Font m_default_font;

  sf::Music m_background_music;

  sf::Texture m_cow_texture;
};

/// Test the resources class
void test_resources() noexcept;

#endif // SFML_RESOURCES_H
