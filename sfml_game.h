#ifndef SFML_sfml_game_H
#define SFML_sfml_game_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "sfml_menu_screen.h"
#include "game.h"
#include "sfml_game_delegate.h"
#include "game_state.h"
#include "sfml_camera.h"
#include "sfml_window_manager.h"
#include "sfml_pause_overlay.h"
#include "sfml_shop_overlay.h"
#include "sfml_save_overlay.h"
#include "sfml_zen_bar.h"

//TODO: decrease the number of member functions and member variables
class sfml_game //!OCLINT indeed to big, will need to simplify
{

public:
  /// Constructor
  /// @param window_width width of the game window in pixels
  /// @param window_height height of the game window in pixels
  /// @param delegate an object that can modify sfml_game at certain times
  sfml_game(const sfml_game_delegate &delegate = sfml_game_delegate(),
            const std::vector<tile>& tiles = create_test_default_tiles(),
            const std::vector<agent>& agents = create_default_agents()
  );

  /// Destructor, is called when sfml_game is destroyed
  ~sfml_game();

  ///@param game state to change to
  void close(game_state s);

  ///Close the game
  void close();

  /// Run the game until it is closed in any way.
  void exec();

  /// Get tile color and outline functions
  sf::Color get_fill_color(tile_type tile);

  /// Take a read-only peek at the game logic
  const game& get_game() const noexcept { return m_game; }

  sf::Color get_outline_color(tile_type tile);

  /// Get how many times the sfml_game has been displayed on screen.
  /// Will be approximately 60 times per second.
  int get_n_displayed() const noexcept { return m_n_displayed; }

  /// Move a selected tile randomly. Will do nothing if no tile is selected.
  void move_selected_tile_randomly();

  ///Select a random tile
  void select_random_tile();

  /// Stop the music
  void stop_music();

  void arrows(bool b, const sf::Event &event);

  bool m_clicked_tile = false;

  int m_timer = 0;

  tile &getTileById(const std::vector<int> &tile_id);

  void control_tile(bool b, const sf::Event &event, tile &t);
  void tile_move_ctrl(const sf::Event &event, tile &t);

  double m_tile_speed = 1; // 112/tile_speed must be a whole number!

  void color_tile_shape(sf::RectangleShape &sfml_tile, const tile &t);
  void color_shape(sf::RectangleShape &sfml_tile, sf::Color c1, sf::Color c2);
  sf::Color m_outline;

  void setup_text();

  bool check_collision(double x, double y);
  std::vector<int> get_collision_id(double x, double y) const;

  /// Check if the tile will colide with another tile if it moves in given
  /// direction
  /// @param Direction: 1 = /\, 2 = >, 3 = \/, 4 = <
  bool will_colide(int direction, tile &t);

  void exec_tile_move(std::vector<int> selected);

//  std::vector<int> m_temp_id;

  void manage_timer();

  void confirm_move();
  void follow_tile();

  bool check_merge(tile &t1, tile &t2);

  void switch_collide(tile& t, int direction);
  void try_rotate(tile& t, bool cc);

  /// @param Direction: 1 = /\, 2 = >, 3 = \/, 4 = <
  sf::Vector2f get_direction_pos(int direction, tile& t, double plus);

  void set_agent_sprite(const agent& a, sf::Sprite& sprite);
  void set_tile_sprite(const tile &t, sf::Sprite &sprite);

  void ben_ik_een_spin();

  void start_music();

  void load_game(const std::string &filename);

private:
  // Functions to display tiles and agents on the screen
  void display_tile(const tile& t);
  void display_agent(const agent& a);

  /// Background music file object
  sf::Music &m_background_music;

  sf::Music &m_ben_ik_een_spin;

  sound_type m_sound_type;

  sf::SoundBuffer m_soundbuffer;

  sf::Sound m_sound;

  void play_sound();

  /// an object that can modify sfml_game at certain times
  sfml_game_delegate m_delegate;

  /// The game logic
  game m_game;

  /// The number of times the sfml_game is displayed on screen
  /// Should be approx 60 times per second
  int m_n_displayed{0};

  /// The window the sfml_game is rendered to
  sf::RenderWindow& m_window;

  /// Display all shapes on the window
  void display();

  ///Process an SFML event
  void process_event(const sf::Event& event);

  /// Handle all events each game frame, for example,
  /// game logic, keyboard and mouse input and the actions
  /// of the sfml_game_delegate
  /// Will be run approx 60 times per second
  /// and increase m_n_displayed
  void process_events();

  /// Process all input from the user: mouse and keyboard
  void process_input();

  /// Reset the input when game state changes
  void reset_input();

  /// Process keyboard input from the user
  ///@param event the SFML keyboard event that needs to be processed
  void process_keyboard_input(const sf::Event &event);

  /// Process mouse input from the user
  ///@param event the SFML mouse event that needs to be processed
  void process_mouse_input(const sf::Event &event);

  sfml_camera m_camera;

  sf::RectangleShape m_essence_symbol;

  sfml_zen_bar m_zen_bar;

  void setup_tickcounter_text();

  void setup_selected_text();

  void update_selected_text();

  void setup_essence_symbol();

  void display_essence_symbol();

  void display_essence();

  sf::Font m_debug_font;

  sf::Text m_tickcounter_text;

  sf::Text m_selected_text;

  sf::Vector2i m_prev_mouse_pos;
  double m_mouse_speed;

  sfml_pause_overlay m_pause_screen;

  sfml_shop_overlay m_shop_overlay;

  sfml_button m_shop_button;

  sfml_save_overlay m_save_screen;

  double m_zoom_state = 1;

};

///Test the sfml_game class
void test_sfml_game();

int vectortoint(std::vector<int> v);

#endif // SFML_sfml_game_H
