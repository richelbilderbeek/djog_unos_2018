#ifndef SFML_sfml_game_H
#define SFML_sfml_game_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "sfml_game_delegate.h"
#include "game.h"

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

  void arrows(bool b, const sf::Event& event);

  std::vector<int> m_selected;

  bool clicked_tile = false;

  int vectortoint(std::vector<int> v);

private:

  ///Background music file object
  sf::Music m_background_music;

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

  ///Process keyboard input from the user
  ///@param event the SFML keyboard event that needs to be processed
  void process_keyboard_input(const sf::Event& event);

  ///Process mouse input from the user
  ///@param event the SFML mouse event that needs to be processed
  void process_mouse_input(const sf::Event& event);

  bool movecam_r = false;
  bool movecam_l = false;
  bool movecam_u = false;
  bool movecam_d = false;
};

#endif // SFML_sfml_game_H
