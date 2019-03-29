#ifndef SFML_SAVE_OVERLAY_H
#define SFML_SAVE_OVERLAY_H

#include "sfml_button.h"
#include "sfml_text_input.h"
#include "game_state.h"
#include "game.h"
#include <SFML/Graphics.hpp>

class sfml_save_overlay
{
public:

    sfml_save_overlay(game &game);

    void exec();

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void set_positions();

    void draw_objects();

private:

    sf::RenderWindow& m_window;

    sf::Font m_font;

    sf::Text m_header;

    sf::RectangleShape m_bg_rect;

    sfml_button m_button1;

    sfml_text_input m_name_input;

    game &m_game;

};

#endif // SFML_SAVE_OVERLAY_H
