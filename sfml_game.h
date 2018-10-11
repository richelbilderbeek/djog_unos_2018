#ifndef SFML_sfml_game_H
#define SFML_sfml_game_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

#include "sfml_game_delegate.h"
#include "game.h"


using namespace sf;
using namespace std;

enum GameState {TitleScreen, MenuScreen, AboutScreen, Playing};

class sfml_game
{
public:

  ///Constructor
  /// @param window_width width of the game window in pixels
  /// @param window_height height of the game window in pixels
  /// @param delegate an object that can modify sfml_game at certain times
  sfml_game(
    const int window_width = 800,
    const int window_height = 600,
    const sfml_game_delegate& delegate = sfml_game_delegate()


  );

  ///Destructor, is called when sfml_game is destroyed
  ~sfml_game();

  ///Close the game
  void close();

  ///Run the game until it is closed in any way.
  void exec();

  ///Get how many times the sfml_game has been displayed on screen.
  ///Will be approximately 60 times per second.
  int get_n_displayed() const noexcept { return m_n_displayed; }

  ///Stop the music
  void stop_music();

  //Show to menu
  void show_title();

  void arrows(bool b, const sf::Event& event);

  bool space_pressed = false;

  std::vector<int> m_selected;

  bool clicked_tile = false;

  int vectortoint(std::vector<int> v);

  int m_timer = 0;

  tile& getTileById(std::vector<int> tile_id);

  void tile_movement(bool b, const sf::Event& event, tile& t);
  void tile_move_ctrl(const sf::Event& event, tile& t);

  double tile_speed = 1; // 115/tile_speed must be a whole number!

  void color_tile_shape(sf::RectangleShape& sfml_tile, const tile& t);
  void color_shape(sf::RectangleShape& sfml_tile, sf::Color c1, sf::Color c2);
  sf::Color outline;

  void setup_text();

  bool check_collision(double x, double y);

  /// Check if the tile will colide with another tile if it moves in given direction
  /// @param Direction: 1 = /\, 2 = >, 3 = \/, 4 = <
  bool will_colide(int direction, tile& t);

  void exec_tile_move(std::vector<int> selected);

  std::vector<int> m_temp_id;

  void manage_timer();

  void confirm_move();
  void follow_tile();

  sf::Vector2i m_screen_center;

private:

  ///Background music file object
  sf::Music m_background_music;

  ///Sate of Game
  GameState m_game_state = Playing;

  ///Camera position in the x direction
  ///If positive, camera is moved right of the origin
  double m_camera_x{-100.0};

  ///Camera position in the y direction
  ///If positive, camera is moved down of the origin
  double m_camera_y{-100.0};

  ///an object that can modify sfml_game at certain times
  sfml_game_delegate m_delegate;

  ///The game logic
  game m_game;

  ///The number of times the sfml_game is displayed on screen
  ///Should be approx 60 times per second
  int m_n_displayed{0};

  ///The window the sfml_game is rendered to
  sf::RenderWindow m_window;

  ///Display all shapes on the window
  void display();

  ///Moves the camera
  void move_camera(sf::Vector2f offset);

  ///Handle all events each game frame, for example,
  ///game logic, keyboard and mouse input and the actions
  ///of the sfml_game_delegate
  ///Will be run approx 60 times per second
  ///and increase m_n_displayed
  void process_events();

  ///Process all input from the user: mouse and keyboard
  void process_input();

  ///Reset the input when game state changes
  void reset_input();

  ///Process keyboard input from the user
  ///@param event the SFML keyboard event that needs to be processed
  void process_keyboard_input(const sf::Event& event);

  ///Process mouse input from the user
  ///@param event the SFML mouse event that needs to be processed
  void process_mouse_input(const sf::Event& event);



  ///Draw Text
  Text titleScreenText;
  Text mainMenuScreenText;
  Text aboutScreenText;
  //Font
  Font m_font;

  bool movecam_r = false;
  bool movecam_l = false;
  bool movecam_u = false;
  bool movecam_d = false;
};

#endif // SFML_sfml_game_H
