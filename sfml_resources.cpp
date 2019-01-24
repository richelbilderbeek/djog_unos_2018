#include "sfml_resources.h"

#include "agent_type.h"
#include "agent.h"

#include <cassert>

#include <QFile>

sfml_resources *sfml_resources::m_instance = nullptr; //!OCLINT static accepted singleton

sfml_resources::sfml_resources() { //!OCLINT must be shorter
  // Background music
  {
    // Re-create resource at executable's location
    QFile f(":/nature_zen/resources/background_music.ogg");
    f.copy("background_music.ogg");
    if (!m_background_music.openFromFile("background_music.ogg")) {
      throw std::runtime_error("Cannot find music file 'background_music.ogg'");
    }
  }
  {
    // Re-create resource at executable's location
    QFile f(":/nature_zen/resources/title_music.ogg");
    f.copy("title_music.ogg");
    if (!m_title_music.openFromFile("title_music.ogg")) {
      throw std::runtime_error("Cannot find music file 'title_music.ogg'");
    }
  }
  {
    // Re-create resource at executable's location
    QFile f(":/nature_zen/resources/ben_ik_een_spin.ogg");
    f.copy("ben_ik_een_spin.ogg");
    if (!m_benikeenspin.openFromFile("ben_ik_een_spin.ogg")) {
      throw std::runtime_error("Cannot find music file 'ben_ik_een_spin.ogg'");
    }
  }
  // bacterium texture
  {
    QFile f(":/nature_zen/resources/bacterium.png");
    f.copy("bacterium.png");
    if (!m_bacterium_texture.loadFromFile("bacterium.png")) {
      throw std::runtime_error("Cannot find image file 'bacterium.png'");
    }
  }
  // grass texture
  {
    QFile f(":/nature_zen/resources/grass.png");
    f.copy("grass.png");
    if (!m_grass_texture.loadFromFile("grass.png"))
      throw std::runtime_error("Cannot find image file 'grass.png'");
  }
  // spider texture
  {
    QFile f(":/nature_zen/resources/spider.png");
    f.copy("spider.png");
    if (!m_spider_texture.loadFromFile("spider.png"))
      throw std::runtime_error("Cannot find image file 'spider.png'");
  }
  // cow texture
  {
    QFile f(":/nature_zen/resources/cow.png");
    f.copy("cow.png");
    if (!m_cow_texture.loadFromFile("cow.png"))
      throw std::runtime_error("Cannot find image file 'cow.png'");
  }
  // Fish texture
  {
    QFile f(":/nature_zen/resources/fish.png");
    f.copy("fish.png");
    if (!m_fish_texture.loadFromFile("fish.png"))
      throw std::runtime_error("Cannot find image file 'fish.png'");
  }
    // Goat texture
    {
      QFile f(":/nature_zen/resources/mountain_goat.jpg");
      f.copy("mountain_goat.jpg");
      if (!m_goat_texture.loadFromFile("mountain_goat.jpg"))
        throw std::runtime_error("Cannot find image file 'mountain_goat.jpg'");
    }
  // crocodile texture
  {
    QFile f(":/nature_zen/resources/crocodile.png");
    f.copy("crocodile.png");
    if (!m_crocodile_texture.loadFromFile("crocodile.png"))
      throw std::runtime_error("Cannot find image file crocodile.png");
  }
  // tree texture
  {
    QFile f(":/nature_zen/resources/tree.png");
    f.copy("tree.png");
    if (!m_tree_texture.loadFromFile("tree.png"))
      throw std::runtime_error("Cannot find image file tree.png");
  }
  // bird texture
  {
    QFile f(":/nature_zen/resources/flying_bird.png");
    f.copy("flying_bird.png");
    if (!m_bird_texture.loadFromFile("flying_bird.png"))
      throw std::runtime_error("Cannot find image file flying_bird.png");
  }
  // None texture
  {
    QFile f(":/nature_zen/resources/none_agent.png");
    f.copy("none_agent.png");
    if (!m_none_texture.loadFromFile("none_agent.png"))
      throw std::runtime_error("Cannot find image file 'none_agent.png'");
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
  {
    QFile f(":/nature_zen/resources/zen_font.ttf");
    f.copy("zen_font.ttf");
    // Set up font
    if (!m_title_font.loadFromFile("zen_font.ttf")) {
      throw std::runtime_error("Cannot find font file zen_font.ttf");
    }
  }
  {
    QFile f(":/nature_zen/resources/title_screen_background.jpg");
    f.copy("title_screen_background.jpg");
    // Set up font
    if (!m_background_image.loadFromFile("title_screen_background.jpg")) {
      throw std::runtime_error("Cannot find image file title_screen_background.jpg");
    }
  }
  {
    QFile f(":/nature_zen/resources/zen_bar.png");
    f.copy("zen_bar.png");
    // Set up font
    if (!m_zen_bar_texture.loadFromFile("zen_bar.png")) {
      throw std::runtime_error("Cannot find image file zen_bar.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/zen_indicator.png");
    f.copy("zen_indicator.png");
    // Set up font
    if (!m_zen_ind_texture.loadFromFile("zen_indicator.png")) {
      throw std::runtime_error("Cannot find image file zen_indicator.png");
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

Texture &sfml_resources::get_agent_sprite(const agent &a) noexcept {
  switch (a.get_type()) {
    case agent_type::bacterium:
      return m_bacterium_texture;
    case agent_type::cow:
      return m_cow_texture;
    case agent_type::crocodile:
      return m_crocodile_texture;
    case agent_type::fish:
      return m_fish_texture;
    case agent_type::grass:
      return m_grass_texture;
    case agent_type::tree:
      return m_tree_texture;
    case agent_type::goat:
      return m_goat_texture;
    case agent_type::spider:
      return m_spider_texture;
    case agent_type::bird:
      return m_bird_texture;
    default:
      return m_none_texture;
  }
}

void test_sfml_resources() //!OCLINT tests may be long
{
  sfml_resources &resources = sfml_resources::get();
  // Music must have a length
  {
    Music &music = resources.get_background_music();
    assert(music.getDuration().asMilliseconds() > 0.0);
  }
  // Music must have a length
  {
    Texture &texture = resources.get_agent_sprite(agent(agent_type::cow));
    assert(texture.getSize().x > 0);
    assert(texture.getSize().y > 0);
  }
  //#define FIX_ISSUE_225
  #ifdef FIX_ISSUE_225
  // Can get the sprite of an agent_type
  {
    assert(resources.get_texture(agent_type::bacterium).getSize().x > 0);
    assert(resources.get_texture(agent_type::cow).getSize().x > 0);
    assert(resources.get_texture(agent_type::crocodile).getSize().x > 0);
    assert(resources.get_texture(agent_type::fish).getSize().x > 0);
    assert(resources.get_texture(agent_type::grass).getSize().x > 0);
    assert(resources.get_texture(agent_type::tree).getSize().x > 0);
    assert(resources.get_texture(agent_type::goat).getSize().x > 0);
  }
  #endif // FIX_ISSUE_225
}
