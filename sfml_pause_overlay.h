#ifndef SFML_PAUSE_OVERLAY_H
#define SFML_PAUSE_OVERLAY_H

#include "sfml_button.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>

class sfml_pause_overlay
{
public:

    sfml_pause_overlay(const int close_at);

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

    sfml_button m_button1;
    sfml_button m_button2;

    int m_close_at;

};

#endif // SFML_PAUSE_OVERLAY_H
