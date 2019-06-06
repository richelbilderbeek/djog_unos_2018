#ifndef SFML_GAMEOVER_SCREEN_H
#define SFML_GAMEOVER_SCREEN_H

#include "sfml_button.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class sfml_gameover_screen
{
public:

    sfml_gameover_screen(const int close_at);

    void exec();

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void stop_music();

    void set_positions();

private:

    sf::Music &m_end_music;

    sf::RenderWindow& m_window;

    sf::Font m_font;

    sf::Text m_header;

    sf::Sprite m_zen_bar;
    sf::Sprite m_zen_icon;

    sfml_button m_button1;

    int m_close_at;

};

#endif // SFML_GAMEOVER_SCREEN_H
