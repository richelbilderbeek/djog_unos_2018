#ifndef SFML_TITLE_SCREEN_H
#define SFML_TITLE_SCREEN_H

#include <sfml/Graphics.hpp>

class sfml_title_screen
{

public:

    sfml_title_screen();

    void exec();

    void change_game_state();

private:

    sf::RenderWindow m_window;
    sf::Font m_font;
    sf::Text title_text;
    sf::Sprite m_bg_sprite;

};

#endif // SFML_TITLE_SCREEN_H
