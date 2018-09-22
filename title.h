#ifndef TITLE_H
#define TITLE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class title
{
public:

  ///Constructor
  /// @param window_width width of the game window in pixels
  /// @param window_height height of the game window in pixels
  /// @param delegate an object that can modify sfml_game at certain times
  title(
    const int window_width = 800,
    const int window_height = 600
  );

  ///Destructor, is called when sfml_game is destroyed
  ~title();

  ///Close the game
  void close();

  ///Run the game until it is closed in any way.
  void exec();

  ///Stop the music
  void stop_music();

private:

  ///Background music file object
  sf::Music m_background_music;

  ///Camera position in the x direction
  ///If positive, camera is moved right of the origin
  double m_camera_x{0.0};

  ///Camera position in the y direction
  ///If positive, camera is moved down of the origin
  double m_camera_y{0.0};

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

};

#endif // TITLE_H
