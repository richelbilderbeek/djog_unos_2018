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

  double m_x = {0};
  double m_y = {0};

  void display();
  void update();

  sf::RenderWindow& m_window;

  sf::Text m_header;

  sf::Clock deltaclock;

  sf::Time deltatime;

  sf::Text m_text;

  sf::Text m_copyright_text;

  int m_close_at;

  sf::Font m_font;

  sf::Font m_zen_font;  

  sf::Sprite m_zen_bar;
  sf::Sprite m_zen_icon;
  
  void display_assets();
  void prepare_assets();

};

std::vector<std::string> get_team_names() noexcept;

std::string get_team_name_string();

#endif // SFML_ABOUT_SCREEN_H
