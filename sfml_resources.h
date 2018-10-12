#ifndef SFML_RESOURCES_H
#define SFML_RESOURCES_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

/// Singleton class to access all resources
class sfml_resources {
public:
  /// Access the resources
  static sfml_resources &get();

  sf::Music &get_background_music() noexcept { return m_background_music; }

  sf::Texture &get_cow_texture() noexcept { return m_cow_texture; }

private:
  /// Do not construct a sfml_resources,
  /// use sfml_resources::get() instead
  sfml_resources();

  static sfml_resources *m_instance;

  sf::Music m_background_music;

  sf::Texture m_cow_texture;
};

/// Test the resources class
void test_resources() noexcept;

#endif // SFML_RESOURCES_H
