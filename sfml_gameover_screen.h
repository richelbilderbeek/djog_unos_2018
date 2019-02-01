#ifndef SFML_GAMEOVER_SCREEN_H
#define SFML_GAMEOVER_SCREEN_H

#include "sfml_button.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>

class sfml_gameover_screen
{
public:

    sfml_gameover_screen(const int close_at);

    void exec();

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void set_positions();

private:

    sf::RenderWindow& m_window;

    sf::Font m_font;

    sf::Text m_header;

    sfml_button m_button1;

    int m_close_at;

};

#endif // SFML_GAMEOVER_SCREEN_H