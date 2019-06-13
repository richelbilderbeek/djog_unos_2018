#ifndef SFML_RESOURCES_H
#define SFML_RESOURCES_H

#include "agent.h"
#include "sound_type.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <QDir>
#include <QFile>
#include "tile_type.h"

/// Test the sfml_resources class
void test_sfml_resources();

/// Singleton class to access all resources
//TODO: make this a member variable of sfml_game
class sfml_resources { //!OCLINT ignore too many fields for now
public:
  /// Access the resources
  static sfml_resources &get();

  sf::Music &get_background_music() noexcept { return m_background_music; }

  sf::Music &get_title_music() noexcept { return m_title_music; }

  sf::Music &get_end_music() noexcept { return m_end_music; }

  /// Get the appropriate soundbuffer
  sf::SoundBuffer& get_soundbuffer(const sound_type st);

  sf::SoundBuffer& random_animal_sound();

  sf::Font& get_default_font() noexcept { return m_default_font; }

  sf::Font& get_title_font() noexcept { return m_title_font; }

  sf::Texture &get_background_image() noexcept { return m_background_image; }

  sf::Texture &get_agent_sprite(const agent_type t ) noexcept;

  sf::Texture &get_agent_sprite(const agent &a ) noexcept;

  sf::Texture &get_zen_bar() noexcept { return m_zen_bar_texture; }

  sf::Texture &get_zen_ind() noexcept { return m_zen_ind_texture; }

  sf::Texture &get_zen_title() noexcept { return m_zen_title; }

  sf::Texture &get_tile_sprite(const tile& t) noexcept;

//  sf::Texture &get_tile_sprite_portrait(const tile_type t) noexcept;

  sf::Texture &get_tile_sprite(const tile_type t) noexcept;

  sf::Texture& get_essence_texture() noexcept { return m_essence_texture; }

private:
  /// Do not construct a sfml_resources,
  /// use sfml_resources::get() instead
  sfml_resources();

  static sfml_resources *m_instance; //!OCLINT accepted for now

  QString m_resource_folder;

  sf::Font m_default_font;

  sf::Font m_title_font;

  sf::Music m_background_music;

  sf::Music m_title_music;

  sf::SoundBuffer m_tile_collission_soundbuffer;

  sf::SoundBuffer m_tile_move_soundbuffer;

  sf::SoundBuffer m_cow_sound;

  sf::SoundBuffer m_horse_sound;

  sf::SoundBuffer m_lion_sound;

  sf::SoundBuffer m_owl_sound;

  sf::Music m_end_music;

  sf::Music m_game_music;

  sf::Texture m_cow_texture;

  sf::Texture m_fish_texture;

  sf::Texture m_crocodile_texture;

  sf::Texture m_tree_texture;

  sf::Texture m_squirrel_texture;

  sf::Texture m_grass_texture;

  sf::Texture m_sun_flower_texture;

  sf::Texture m_whale_texture;

  sf::Texture m_sunflower_texture;

  sf::Texture m_goat_texture;

  sf::Texture m_none_texture;

  sf::Texture m_spider_texture;

  sf::Texture m_plankton_texture;

  sf::Texture m_worm_texture;

  sf::Texture m_giraffe_texture;

  sf::Texture m_lion_texture;

  sf::Texture m_octopus_texture;

  sf::Texture m_polar_bear_texture;

  sf::Texture m_snake_texture;

  sf::Texture m_foxgloves_texture;

  sf::Texture m_background_image;

  sf::Texture m_bird_texture;

  sf::Texture m_cactus_texture;

  sf::Texture m_chameleon_texture;

  sf::Texture m_venus_fly_trap_texture;

  sf::Texture m_zen_ind_texture;

  sf::Texture m_zen_bar_texture;

  sf::Texture m_empty_tile;

  sf::Texture m_zen_title;

  sf::Texture m_arctic_laying;
  sf::Texture m_arctic_standing;

  sf::Texture m_beach_laying;
  sf::Texture m_beach_standing;

  sf::Texture m_desert_laying;
  sf::Texture m_desert_standing;

  sf::Texture m_dunes_laying;
  sf::Texture m_dunes_standing;

  sf::Texture m_grassland_laying;
  sf::Texture m_grassland_standing;

  sf::Texture m_hills_laying;
  sf::Texture m_hills_standing;

  sf::Texture m_mangrove_laying;
  sf::Texture m_mangrove_standing;

  sf::Texture m_mountain_laying;
  sf::Texture m_mountain_standing;

  sf::Texture m_rainforest_laying;
  sf::Texture m_rainforest_standing;

  sf::Texture m_savannah_laying;
  sf::Texture m_savannah_standing;

  sf::Texture m_swamp_laying;
  sf::Texture m_swamp_standing;

  sf::Texture m_tundra_laying;
  sf::Texture m_tundra_standing;

  sf::Texture m_water_laying;
  sf::Texture m_water_standing;

  sf::Texture m_woods_laying;
  sf::Texture m_woods_standing;

  sf::Texture m_corpse_texture;

  sf::Texture m_essence_texture;

  friend void test_sfml_resources();
};


#endif // SFML_RESOURCES_H
