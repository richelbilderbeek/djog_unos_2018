#ifndef SFML_MENU_SCREEN_H
#define SFML_MENU_SCREEN_H

#include "sfml_button.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>

class sfml_menu_screen
{
public:

    sfml_menu_screen(const int close_at);

    void exec();

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void set_positions();

private:

    sf::RenderWindow& m_window;

    sf::Font m_font;

    sf::Text m_main_text;

    sfml_button m_button1;

    int m_close_at;

};

#endif // SFML_MENU_SCREEN_H
