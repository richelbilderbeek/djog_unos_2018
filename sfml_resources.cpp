#include "sfml_resources.h"
#include "agent_type.h"
#include "agent.h"
#include "tile_type.h"
#include "tile.h"
#include "sfml_game.h"

#include <cassert>

sfml_resources *sfml_resources::m_instance = nullptr; //!OCLINT static accepted singleton

sfml_resources::sfml_resources() { //!OCLINT must be shorter
  // Background music
  {
    // background music
    QFile f(":/nature_zen/resources/Theme_Music.ogg");
    f.copy("Theme_Music.ogg");
    if (!m_background_music.openFromFile("Theme_Music.ogg")) {
      throw std::runtime_error("Cannot find music file "
                               "'Theme_Music.ogg'");
    }
  }
  {
    // title music
    QFile f(":/nature_zen/resources/Title_Music.ogg");
    f.copy("Title_Music.ogg");
    if (!m_title_music.openFromFile("Title_Music.ogg")) {
      throw std::runtime_error("Cannot find music file 'Title_Music.ogg'");
    }
  }
  {
    // ik ben een spin
    QFile f(":/nature_zen/resources/End_Music.ogg");
    f.copy("End_Music.ogg");
    if (!m_end_music.openFromFile("End_Music.ogg")) {
      throw std::runtime_error("Cannot find music file 'End_Music.ogg'");
    }
  }
  {
    // tile merge
    QFile f(":/nature_zen/resources/tile_merge.wav");
    f.copy("tile_merge.wav");
    if (!m_tile_collission_soundbuffer.loadFromFile("tile_merge.wav")) {
      throw std::runtime_error("Cannot find music file 'tile_merge.wav'");
    }
  }
  {
    // tile move
    QFile f(":/nature_zen/resources/tile_move.wav");
    f.copy("tile_move.wav");
    if (!m_tile_move_soundbuffer.loadFromFile("tile_move.wav")) {
      throw std::runtime_error("Cannot find music file 'tile_move.wav'");
    }
  }
  {
    // tile rotate
    QFile f(":/nature_zen/resources/tile_rotate.wav");
    f.copy("tile_rotate.wav");
    if (!m_tile_rotate_soundbuffer.loadFromFile("tile_rotate.wav")) {
      throw std::runtime_error("Cannot find music file 'tile_rotate.wav'");
    }
  }
  {
    // cow
    QFile f(":/nature_zen/resources/cow_01.wav");
    f.copy("cow_01.wav");
    if (!m_cow_sound.loadFromFile("cow_01.wav")) {
      throw std::runtime_error("Cannot find music file 'cow_01.wav'");
    }
  }  
  {
    // horse
    QFile f(":/nature_zen/resources/horse_09.wav");
    f.copy("horse_09.wav");
    if (!m_horse_sound.loadFromFile("horse_09.wav")) {
      throw std::runtime_error("Cannot find music file 'horse_09.wav'");
    }
  }
  {
    // lion
    QFile f(":/nature_zen/resources/lion_04.wav");
    f.copy("lion_04.wav");
    if (!m_lion_sound.loadFromFile("lion_04.wav")) {
      throw std::runtime_error("Cannot find music file 'lion_04.wav'");
    }
  }
  {
    // owl
    QFile f(":/nature_zen/resources/owl_37.wav");
    f.copy("owl_37.wav");
    if (!m_owl_sound.loadFromFile("owl_37.wav")) {
      throw std::runtime_error("Cannot find music file 'owl_37.wav'");
    }
  }
   // plankton texture
  {
    QFile f(":/nature_zen/resources/plankton.png");
    f.copy("plankton.png");
    if (!m_plankton_texture.loadFromFile("plankton.png")) {
      throw std::runtime_error("Cannot find image file 'plankton.png'");
    }
  }
  // foxgloves texture
  {
    QFile f(":/nature_zen/resources/foxgloves.png");
    f.copy("foxgloves.png");
    if (!m_foxgloves_texture.loadFromFile("foxgloves.png")) {
      throw std::runtime_error("Cannot find image file 'foxgloves.png'");
    }
  }
  // worm texture
    {
      QFile f(":/nature_zen/resources/worm.png");
      f.copy("worm.png");
      if (!m_worm_texture.loadFromFile("worm.png")) {
        throw std::runtime_error("Cannot find image file 'worm.png'");
      }
    }
    // giraffe texture
    {
        QFile f(":/nature_zen/resources/giraffe.png");
        f.copy("giraffe.png");
        if (!m_giraffe_texture.loadFromFile("giraffe.png")) {
          throw std::runtime_error("Cannot find image file 'giraffe.png'");
        }
    }
    // lion texture
    {
        QFile f(":/nature_zen/resources/lion.png");
        f.copy("lion.png");
        if (!m_lion_texture.loadFromFile("lion.png")) {
          throw std::runtime_error("Cannot find image file 'lion.png'");
    }
    }
  // grass texture
  {
    QFile f(":/nature_zen/resources/grass.png");
    f.copy("grass.png");
    if (!m_grass_texture.loadFromFile("grass.png"))
      throw std::runtime_error("Cannot find image file 'grass.png'");
  }
  // grass texture
  {
    QFile f(":/nature_zen/resources/sunflower.png");
    f.copy("sunflower.png");
    if (!m_sun_flower_texture.loadFromFile("sunflower.png"))
      throw std::runtime_error("Cannot find image file 'sunflower.png'");
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
  // Whale texture
  {
    QFile f(":/nature_zen/resources/whale.png");
    f.copy("whale.png");
    if (!m_whale_texture.loadFromFile("whale.png"))
      throw std::runtime_error("Cannot find image file 'whale.png'");
  }
  // octopus texture
  {
    QFile f(":/nature_zen/resources/octopus.png");
    f.copy("octopus.png");
    if (!m_octopus_texture.loadFromFile("octopus.png"))
      throw std::runtime_error("Cannot find image file 'octopus.png'");
  }
  // polar_bear texture
  {
    QFile f(":/nature_zen/resources/polar_bear.png");
    f.copy("polar_bear.png");
    if (!m_polar_bear_texture.loadFromFile("polar_bear.png"))
      throw std::runtime_error("Cannot find image file 'polar_bear.png'");
  }
  // Goat texture
  {
    QFile f(":/nature_zen/resources/mountain_goat.png");
    f.copy("mountain_goat.png");
    if (!m_goat_texture.loadFromFile("mountain_goat.png"))
      throw std::runtime_error("Cannot find image file 'mountain_goat.png'");
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
  // snake texture
  {
    QFile f(":/nature_zen/resources/snake.png");
    f.copy("snake.png");
    if (!m_snake_texture.loadFromFile("snake.png"))
      throw std::runtime_error("Cannot find image file snake.png");
  }
  // sunflower texture
  {
    QFile f(":/nature_zen/resources/sunflower.png");
    f.copy("sunflower.png");
    if (!m_sunflower_texture.loadFromFile("sunflower.png"))
      throw std::runtime_error("Cannot find image file sunflower.png");
  }
  // squirrel texture
  {
    QFile f(":/nature_zen/resources/squirrel.png");
    f.copy("squirrel.png");
    if (!m_squirrel_texture.loadFromFile("squirrel.png"))
      throw std::runtime_error("Cannot find image file squirrel.png");
  }
  // venus_fly_trap texture
  {
    QFile f(":/nature_zen/resources/venus_fly_trap.png");
    f.copy("venus_fly_trap.png");
    if (!m_venus_fly_trap_texture.loadFromFile("venus_fly_trap.png"))
      throw std::runtime_error("Cannot find image file venus_fly_trap.png");
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
    if (!m_default_font.loadFromFile("font.ttf")) {
      throw std::runtime_error("Cannot find font file font.ttf");
    }
  }

  {
    QFile f(":/nature_zen/resources/zen_font.ttf");
    f.copy("zen_font.ttf");
    if (!m_title_font.loadFromFile("zen_font.ttf")) {
      throw std::runtime_error("Cannot find font file zen_font.ttf");
    }
  }

  {
    QFile f(":/nature_zen/resources/title_screen_background.png");
    f.copy("title_screen_background.png");
    if (!m_background_image.loadFromFile("title_screen_background.png")) {
      throw std::runtime_error("Cannot find image file title_screen_background.png");
    }
  }

  {
    QFile f(":/nature_zen/resources/zen_bar.png");
    f.copy("zen_bar.png");
    if (!m_zen_bar_texture.loadFromFile("zen_bar.png")) {
      throw std::runtime_error("Cannot find image file zen_bar.png");
    }
  }

  {
    QFile f(":/nature_zen/resources/zen_indicator.png");
    f.copy("zen_indicator.png");
    if (!m_zen_ind_texture.loadFromFile("zen_indicator.png")) {
      throw std::runtime_error("Cannot find image file zen_indicator.png");
    }
  }

  {
    QFile f(":/nature_zen/resources/none_tile.png");
    f.copy("none_tile.png");
    if (!m_empty_tile.loadFromFile("none_tile.png")) {
      throw std::runtime_error("Cannot find image file none_tile.png");
    }
  }


  {
    QFile f(":/nature_zen/resources/zen_title.png");
    f.copy("zen_title.png");
    if (!m_zen_title.loadFromFile("zen_title.png")) {
      throw std::runtime_error("Cannot find image file zen_title.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/cactus.png");
    f.copy("cactus.png");
    if (!m_cactus_texture.loadFromFile("cactus.png")) {
      throw std::runtime_error("Cannot find image file cactus.png");
    }
  }
  //----------------------------------------------------------------------------
  // Land types
  //----------------------------------------------------------------------------
//  {
//    QFile f(":/nature_zen/resources/arctic_laying.png");
//    f.copy("arctic_laying.png");
//    if (!m_arctic_laying.loadFromFile("arctic_laying.png")) {
//      throw std::runtime_error("Cannot find image file arctic_laying.png");
//    }
//  }
  {
    QFile f(":/nature_zen/resources/arctic_standing.png");
    f.copy("arctic_standing.png");
    if (!m_arctic_standing.loadFromFile("arctic_standing.png")) {
      throw std::runtime_error("Cannot find image file arctic_standing.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/beach_laying.png");
    f.copy("beach_laying.png");
    if (!m_beach_laying.loadFromFile("beach_laying.png")) {
      throw std::runtime_error("Cannot find image file beach_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/beach_standing.png");
    f.copy("beach_standing.png");
    if (!m_beach_standing.loadFromFile("beach_standing.png")) {
      throw std::runtime_error("Cannot find image file beach_standing.png");
    }
  }
  /* TODO
  {
    QFile f(":/nature_zen/resources/desert_laying.png");
    f.copy("desert_laying.png");
    if (!m_desert_laying.loadFromFile("desert_laying.png")) {
      throw std::runtime_error("Cannot find image file desert_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/desert_standing.png");
    f.copy("desert_standing.png");
    if (!m_desert_standing.loadFromFile("desert_standing.png")) {
      throw std::runtime_error("Cannot find image file desert_standing.png");
    }
  }
  */
  {
    QFile f(":/nature_zen/resources/dunes_laying.png");
    f.copy("dunes_laying.png");
    if (!m_dunes_laying.loadFromFile("dunes_laying.png")) {
      throw std::runtime_error("Cannot find image file dunes_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/dunes_standing.png");
    f.copy("dunes_standing.png");
    if (!m_dunes_standing.loadFromFile("dunes_standing.png")) {
      throw std::runtime_error("Cannot find image file dunes_standing.png");
    }
  }
  /* TODO
  {
    QFile f(":/nature_zen/resources/grassland_laying.png");
    f.copy("grassland_laying.png");
    if (!m_grassland_laying.loadFromFile("grassland_laying.png")) {
      throw std::runtime_error("Cannot find image file grassland_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/grassland_standing.png");
    f.copy("grassland_standing.png");
    if (!m_grassland_standing.loadFromFile("grassland_standing.png")) {
      throw std::runtime_error("Cannot find image file grassland_standing.png");
    }
  }
  */
  {
    QFile f(":/nature_zen/resources/hills_laying.png");
    f.copy("hills_laying.png");
    if (!m_hills_laying.loadFromFile("hills_laying.png")) {
      throw std::runtime_error("Cannot find image file hills_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/hills_standing.png");
    f.copy("hills_standing.png");
    if (!m_hills_standing.loadFromFile("hills_standing.png")) {
      throw std::runtime_error("Cannot find image file hills_standing.png");
    }
  }
  /*TODO
  {
    QFile f(":/nature_zen/resources/mangrove_laying.png");
    f.copy("mangrove_laying.png");
    if (!m_mangrove_laying.loadFromFile("mangrove_laying.png")) {
      throw std::runtime_error("Cannot find image file mangrove_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/mangrove_standing.png");
    f.copy("mangrove_standing.png");
    if (!m_mangrove_standing.loadFromFile("mangrove_standing.png")) {
      throw std::runtime_error("Cannot find image file mangrove_standing.png");
    }
  }
  */
  {
    QFile f(":/nature_zen/resources/mountain_laying.png");
    f.copy("mountain_laying.png");
    if (!m_mountain_laying.loadFromFile("mountain_laying.png")) {
      throw std::runtime_error("Cannot find image file mountain_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/mountain_standing.png");
    f.copy("mountain_standing.png");
    if (!m_mountain_standing.loadFromFile("mountain_standing.png")) {
      throw std::runtime_error("Cannot find image file mountain_standing.png");
    }
  }
  /*TODO
  {
    QFile f(":/nature_zen/resources/rainforest_laying.png");
    f.copy("rainforest_laying.png");
    if (!m_rainforest_laying.loadFromFile("rainforest_laying.png")) {
      throw std::runtime_error("Cannot find image file rainforest_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/rainforest_standing.png");
    f.copy("rainforest_standing.png");
    if (!m_rainforest_standing.loadFromFile("rainforest_standing.png")) {
      throw std::runtime_error("Cannot find image file rainforest_standing.png");
    }
  }
  */
  /*
  {
    QFile f(":/nature_zen/resources/savannah_laying.png");
    f.copy("savannah_laying.png");
    if (!m_savannah_laying.loadFromFile("savannah_laying.png")) {
      throw std::runtime_error("Cannot find image file savannah_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/savannah_standing.png");
    f.copy("savannah_standing.png");
    if (!m_savannah_standing.loadFromFile("savannah_standing.png")) {
      throw std::runtime_error("Cannot find image file savannah_standing.png");
    }
  }
  */
  /*TODO
  {
    QFile f(":/nature_zen/resources/swamp_laying.png");
    f.copy("swamp_laying.png");
    if (!m_swamp_laying.loadFromFile("swamp_laying.png")) {
      throw std::runtime_error("Cannot find image file swamp_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/essence.png");
    f.copy("essence.png");
    if (!m_essence_texture.loadFromFile("essence.png")) {
      throw std::runtime_error("Cannot find image file essence.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/swamp_standing.png");
    f.copy("swamp_standing.png");
    if (!m_swamp_standing.loadFromFile("swamp_standing.png")) {
      throw std::runtime_error("Cannot find image file swamp_standing.png");
    }
  }
  */
  {
    QFile f(":/nature_zen/resources/tundra_laying.png");
    f.copy("tundra_laying.png");
    if (!m_tundra_laying.loadFromFile("tundra_laying.png")) {
      throw std::runtime_error("Cannot find image file tundra_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/tundra_standing.png");
    f.copy("tundra_standing.png");
    if (!m_tundra_standing.loadFromFile("tundra_standing.png")) {
      throw std::runtime_error("Cannot find image file tundra_standing.png");
    }
  }
  /*TODO
  {
    QFile f(":/nature_zen/resources/water_laying.png");
    f.copy("water_laying.png");
    if (!m_water_laying.loadFromFile("water_laying.png")) {
      throw std::runtime_error("Cannot find image file water_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/water_standing.png");
    f.copy("water_standing.png");
    if (!m_water_standing.loadFromFile("water_standing.png")) {
      throw std::runtime_error("Cannot find image file water_standing.png");
    }
  }
  */
  /*TODO
  {
    QFile f(":/nature_zen/resources/woods_laying.png");
    f.copy("woods_laying.png");
    if (!m_woods_laying.loadFromFile("woods_laying.png")) {
      throw std::runtime_error("Cannot find image file woods_laying.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/woods_standing.png");
    f.copy("woods_standing.png");
    if (!m_woods_standing.loadFromFile("woods_standing.png")) {
      throw std::runtime_error("Cannot find image file woods_standing.png");
    }
  }
  */
   //-------------------------------------------------------------------------
  // Misc other things
  //-------------------------------------------------------------------------
  /*{
    QFile f(":/nature_zen/resources/Essence_29_32.png");
    f.copy("Essence_29_32.png");
    if (!m_essence_texture.loadFromFile("Essence_29_32.png")) {
      throw std::runtime_error("Cannot find image file Essence_29_32.png");
    }
  }*/
  {
    QFile f(":/nature_zen/resources/essence.png");
    f.copy("essence.png");
    if (!m_essence_texture.loadFromFile("essence.png")) {
      throw std::runtime_error("Cannot find image file essence.png");
    }
  }
  {
    QFile f(":/nature_zen/resources/blood-spatter.png");
    f.copy("blood-spatter.png");
    if (!m_corpse_texture.loadFromFile("blood-spatter.png"))
      throw std::runtime_error("Cannot find image file blood-spatter.png");
  }
  {
    QFile f(":/nature_zen/resources/chameleon.png");
    f.copy("chameleon.png");
    if (!m_chameleon_texture.loadFromFile("chameleon.png"))
      throw std::runtime_error("Cannot find image file chameleon.png");
  }
}

sfml_resources &sfml_resources::get() {
  if (!m_instance) {
    m_instance = new sfml_resources();
  }
  assert(m_instance);
  return *m_instance;
}

sf::Texture &sfml_resources::get_agent_sprite(const agent &a) noexcept
{
  return get_agent_sprite(a.get_type());
}

sf::Texture &sfml_resources::get_agent_sprite(const agent_type t) noexcept //!OCLINT indeed too complex
{
  switch (t)
  {
    case agent_type::plankton:
      return m_plankton_texture;
    case agent_type::chameleon:
      return m_chameleon_texture;
    case agent_type::worm:
      return m_worm_texture;
    case agent_type::cow:
      return m_cow_texture;
    case agent_type::crocodile:
      return m_crocodile_texture;
    case agent_type::sunflower:
      return m_sun_flower_texture;
    case agent_type::fish:
      return m_fish_texture;
    case agent_type::whale:
      return m_whale_texture;
    case agent_type::grass:
      return m_grass_texture;
    case agent_type::tree:
      return m_tree_texture;
    case agent_type::goat:
      return m_goat_texture;
    case agent_type::snake:
      return m_snake_texture;
    case agent_type::spider:
      return m_spider_texture;
    case agent_type::squirrel:
      return m_squirrel_texture;
    case agent_type::bird:
      return m_bird_texture;
    case agent_type::lion:
        return m_lion_texture;
    case agent_type::giraffe:
      return m_giraffe_texture;
    case agent_type::foxgloves:
      return m_foxgloves_texture;
    case agent_type::octopus:
      return m_octopus_texture;
    case agent_type::venus_fly_trap:
      return m_venus_fly_trap_texture;
    case agent_type::cactus:
      return m_cactus_texture;
    case agent_type::corpse:
      return m_corpse_texture;
    default:
      return m_none_texture;
  }
}

sf::SoundBuffer& sfml_resources::get_soundbuffer(const sound_type st)
{
  /// Only deal with actual sounds
  assert(st != sound_type::none);

  /// Getting the collision soundfile
  if (st == sound_type::tile_collision)
    return m_tile_collission_soundbuffer;
  if (st == sound_type::tile_move)
    return m_tile_move_soundbuffer;
  if (st == sound_type::tile_rotate)
    return m_tile_rotate_soundbuffer;
  if (st == sound_type::random_animal)
    return random_animal_sound();

  assert(!"Should never come this far."); //!OCLINT accepted idiom
  return m_tile_collission_soundbuffer;
}

sf::SoundBuffer& sfml_resources::random_animal_sound()
{
  const int choose{ random_int(1, 4) };

  switch (choose)
  {
    case 1 :
      return m_cow_sound;
    case 2 :
      return m_horse_sound;
    case 3 :
      return m_lion_sound;
    case 4 :
      return m_owl_sound;
  }
  return m_cow_sound;
}

sf::Texture &sfml_resources::get_tile_sprite(const tile &t) noexcept //!OCLINT too long, needs to be fixed
{
  return get_tile_sprite(t.get_type());
}

sf::Texture &sfml_resources::get_tile_sprite(const tile_type t) noexcept //!OCLINT cannot be simpler
{
  switch (t)
  {
    case tile_type::arctic: return m_arctic_laying;
    case tile_type::beach: return m_beach_laying;
    case tile_type::desert: return m_desert_laying;
    case tile_type::dunes: return m_dunes_laying;
    case tile_type::grassland: return m_grassland_laying;
    case tile_type::hills: return m_hills_laying;
    case tile_type::mangrove: return m_mangrove_laying;
    case tile_type::mountain: return m_mountain_laying;
    case tile_type::rainforest: return m_rainforest_laying;
    case tile_type::savannah: return m_savannah_laying;
    case tile_type::swamp: return m_swamp_laying;
    case tile_type::tundra: return m_tundra_laying;
    case tile_type::water: return m_water_laying;
    case tile_type::woods: return m_woods_laying;
    default:
      break;
  }
  return m_empty_tile;
}

void test_sfml_resources() //!OCLINT tests may be long
{
  sfml_resources &resources = sfml_resources::get();
  // Music must have a length
  {
    sf::Music &music = resources.get_background_music();
    assert(music.getDuration().asMilliseconds() > 0.0);
  }
  // Music must have a length
  {
    sf::Texture &texture = resources.get_agent_sprite(agent(agent_type::cow));
    assert(texture.getSize().x > 0);
    assert(texture.getSize().y > 0);
  }
  // Can get the sprite of an agent_type
  {
    assert(resources.get_agent_sprite(agent_type::cow).getSize().x > 0);
    assert(resources.get_agent_sprite(agent_type::crocodile).getSize().x > 0);
    assert(resources.get_agent_sprite(agent_type::fish).getSize().x > 0);
    assert(resources.get_agent_sprite(agent_type::goat).getSize().x > 0);
    assert(resources.get_agent_sprite(agent_type::grass).getSize().x > 0);
    assert(resources.get_agent_sprite(agent_type::plankton).getSize().x > 0);
    assert(resources.get_agent_sprite(agent_type::tree).getSize().x > 0);
  }
  { /// Testing succesful access to the essence symbol png and its dimensions
    sf::Texture texture{ resources.get_essence_texture() };
    assert(texture.getSize().x > 0);
    assert(texture.getSize().y > 0);
  }
//  assert(resources.get_tile_sprite(tile_type::arctic).getSize().x > 0);
  assert(resources.get_tile_sprite(tile_type::beach).getSize().x > 0);
  //assert(resources.get_tile_sprite(tile_type::desert).getSize().x > 0);TODO
  assert(resources.get_tile_sprite(tile_type::dunes).getSize().x > 0);
  //assert(resources.get_tile_sprite(tile_type::grassland).getSize().x > 0);
  assert(resources.get_tile_sprite(tile_type::hills).getSize().x > 0);
  //assert(resources.get_tile_sprite(tile_type::mangrove).getSize().x > 0);
  assert(resources.get_tile_sprite(tile_type::mountain).getSize().x > 0);
  //assert(resources.get_tile_sprite(tile_type::rainforest).getSize().x > 0);
  //assert(resources.get_tile_sprite(tile_type::savannah).getSize().x > 0);
  //assert(resources.get_tile_sprite(tile_type::swamp).getSize().x > 0);
  assert(resources.get_tile_sprite(tile_type::tundra).getSize().x > 0);
  //assert(resources.get_tile_sprite(tile_type::water).getSize().x > 0);
  //assert(resources.get_tile_sprite(tile_type::woods).getSize().x > 0);
}
