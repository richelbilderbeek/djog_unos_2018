#include "sfml_resources.h"

#include <QFile>
#include <cassert>

sfml_resources *sfml_resources::m_instance = nullptr; //! OCLINT static members are accepted in a Singleton

sfml_resources::sfml_resources() {
  // Background music
  {
    // Re-create resource at executable's location
    QFile f(":/nature_zen/resources/background_music.ogg");
    f.copy("background_music.ogg");
    if (!m_background_music.openFromFile("background_music.ogg")) {
      throw std::runtime_error("Cannot find music file 'background_music.ogg'");
    }
  }
  // bacterie texture
  {
    QFile f(":/nature_zen/resources/bacterie.png");
    f.copy("bacterie.png");
    m_bacterie_texture.loadFromFile("bacterie.png");
  }
  // gras texture
  {
    QFile f(":/nature_zen/resources/gras.png");
    f.copy("gras.png");
    m_gras_texture.loadFromFile("gras.png");
  }
  // cow texture
  {
    QFile f(":/nature_zen/resources/cow.png");
    f.copy("cow.png");
    m_cow_texture.loadFromFile("cow.png");
  }
  // Fish texture
  {
    QFile f(":/nature_zen/resources/fish.png");
    f.copy("fish.png");
    m_fish_texture.loadFromFile("fish.png");
  }
  // crocodile texture
  {
    QFile f(":/nature_zen/resources/crocodile.png");
    f.copy("crocodile.png");
    m_crocodile_texture.loadFromFile("crocodile.png");
  }
  // Resources
  {
    QFile f(":/nature_zen/resources/font.ttf");
    f.copy("font.ttf");
    // Set up font
    if (!m_default_font.loadFromFile("font.ttf")) {
      throw std::runtime_error("Cannot find font file font.ttf");
    }
  }
}

sfml_resources &sfml_resources::get() {
  if (!m_instance) {
    m_instance = new sfml_resources();
  }
  assert(m_instance);
  return *m_instance;
}

void test_resources() noexcept {
  sfml_resources &resources = sfml_resources::get();
  // Music must have a length
  {
    sf::Music &music = resources.get_background_music();
    assert(music.getDuration().asMilliseconds() > 0.0);
  }
  // Music must have a length
  {
    sf::Texture &texture = resources.get_cow_texture();
    assert(texture.getSize().x > 0);
    assert(texture.getSize().y > 0);
  }
}
