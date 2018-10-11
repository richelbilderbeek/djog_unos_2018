#include "sfml_resources.h"

#include <cassert>
#include <QFile>

sfml_resources * sfml_resources::m_instance = nullptr;

sfml_resources::sfml_resources()
{
  //Background music
  {
    //Re-create resource at executable's location
    QFile f(":/nature_zen/resources/background_music.ogg");
    f.copy("background_music.ogg");
    if (!m_background_music.openFromFile("background_music.ogg"))
    {
      throw std::runtime_error("Cannot find music file 'background_music.ogg'");
    }
  }
  //Cow texture
  {
    QFile f(":/nature_zen/resources/cow.png");
    f.copy("cow.png");
    m_cow_texture.loadFromFile("cow.png");
  }
}

sfml_resources& sfml_resources::get()
{
  if (!m_instance)
  {
    m_instance = new sfml_resources();
  }
  assert(m_instance);
  return *m_instance;
}

void test_resources() noexcept
{
  sfml_resources& resources = sfml_resources::get();
  //Music must have a length
  {
    sf::Music& music = resources.get_background_music();
    assert(music.getDuration().asMilliseconds() > 0.0);
  }
  //Music must have a length
  {
    sf::Texture& texture = resources.get_cow_texture();
    assert(texture.getSize().x > 0);
    assert(texture.getSize().y > 0);
  }
}
