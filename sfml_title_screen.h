#ifndef SFML_TITLE_SCREEN_H
#define SFML_TITLE_SCREEN_H

#include "game_state.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class sfml_title_screen
{

public:

    sfml_title_screen(const int close_at);
    ~sfml_title_screen();

    void exec();

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void stop_music();

    void change_game_state();

    void animation();

private:

    sf::Music &m_title_music;
    sf::RenderWindow &m_window;
    sf::Font m_font;
    sf::Text title_text;
    sf::Sprite m_bg_sprite;

    double i = 0;
    bool b = true;

    int m_close_at;

};

#endif // SFML_TITLE_SCREEN_H
