#ifndef SFML_sfml_game_H
#define SFML_sfml_game_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "sfml_menu_screen.h"
#include "game.h"
#include "sfml_game_delegate.h"
#include "game_state.h"
#include "sfml_camera.h"

//TODO: decrease the number of member functions and member variables
class sfml_game //!OCLINT indeed to big, will need to simplify
{

public:
  /// Constructor
  /// @param window_width width of the game window in pixels
  /// @param window_height height of the game window in pixels
  /// @param delegate an object that can modify sfml_game at certain times
  sfml_game(const int window_width = 800, const int window_height = 600,
            const sfml_game_delegate &delegate = sfml_game_delegate(),
            const std::vector<tile>& tiles = create_default_tiles(),
            const std::vector<agent>& agents = create_default_agents()
  );

  /// Destructor, is called when sfml_game is destroyed
  ~sfml_game();

  /// Close the game
  void close();

  /// Run the game until it is closed in any way.
  void exec();

  /// Get how many times the sfml_game has been displayed on screen.
  /// Will be approximately 60 times per second.
  int get_n_displayed() const noexcept { return m_n_displayed; }

  /// Move a selected tile randomly. Will do nothing if no tile is selected.
  void move_selected_tile_randomly();

  ///Select a random tile
  void select_random_tile();

  /// Stop the music
  void stop_music();

  // Show to menu
  void arrows(bool b, const sf::Event &event);

  bool m_clicked_tile = false;

  int m_timer = 0;

  tile &getTileById(const std::vector<int> &tile_id);

  void tile_movement(bool b, const sf::Event &event, tile &t);
  void tile_move_ctrl(const sf::Event &event, tile &t);

  double m_tile_speed = 1; // 115/tile_speed must be a whole number!

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

  sf::Vector2i m_screen_center;

  void change_game_state();

  void load_game_state();

  void check_change_game_state(const sf::Event &event);

  bool check_merge(tile &t1, tile &t2);

  void switch_collide(tile& t, int direction);

  /// @param Direction: 1 = /\, 2 = >, 3 = \/, 4 = <
  sf::Vector2f get_direction_pos(int direction, tile& t, double plus);

  void confirm_tile_move(tile& t, int direction);

  void set_agent_sprite(const agent& a, sf::Sprite& sprite);

  void ben_ik_een_spin();

  void start_music();

private:
  // Functions to display tiles and agents on the screen
  void display_tile(const tile& t);
  void display_agent(const agent& a);

  /// Background music file object
  sf::Music &m_background_music;

  sf::Music &m_ben_ik_een_spin;

  /// State of Game
  game_state m_game_state = game_state::playing;

  /// an object that can modify sfml_game at certain times
  sfml_game_delegate m_delegate;

  /// The game logic
  game m_game;

  /// The number of times the sfml_game is displayed on screen
  /// Should be approx 60 times per second
  int m_n_displayed{0};

  /// The window the sfml_game is rendered to
  sf::RenderWindow m_window;

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

  bool m_is_space_pressed = false;

  sfml_camera m_camera;

  sf::RectangleShape m_zen_bar;
  sf::RectangleShape m_zen_ind;

  void setup_display_score();

};

///Test the sfml_game class
void test_sfml_game();

int vectortoint(std::vector<int> v);

///Get the video mode, which is full-screen
///by default, except on Travis CI
int get_video_mode();

#endif // SFML_sfml_game_H
