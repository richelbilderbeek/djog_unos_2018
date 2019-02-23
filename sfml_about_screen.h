#ifndef SFML_ABOUT_SCREEN_H
#define SFML_ABOUT_SCREEN_H

#include "game_state.h"
#include <SFML/Graphics.hpp>

class sfml_about_screen {
public:
  sfml_about_screen(const int close_at = -1);

  void exec();

  ///@param game state to change to
  void close(game_state s);

  ///Close the game
  void close();

private:

  sf::RenderWindow& m_window;

  sf::Text m_header;

  sf::Text m_text;

  int m_close_at;

  sf::Font m_font;

  sf::Font m_zen_font;  
  sf::RectangleShape m_zen_bar;
  sf::RectangleShape m_zen_ind;
  
//   void sfml_about_screen::display_assets();

};

std::vector<std::string> get_team_names() noexcept;

std::string get_team_name_string();

#endif // SFML_ABOUT_SCREEN_H
