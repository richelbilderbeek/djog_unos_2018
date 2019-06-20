#ifndef SFML_LOAD_SCREEN_H
#define SFML_LOAD_SCREEN_H

#include "sfml_button.h"
#include "game_state.h"
#include "sfml_scroll_box.h"
#include <SFML/Graphics.hpp>

class sfml_load_screen
{
public:

    sfml_load_screen();

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

    sfml_button m_new_game;

    sfml_scroll_box m_list;

    std::vector<sfml_button> m_saves;
};

/// Test the loading screen
void test_sfml_load_screen();

#endif // SFML_LOAD_SCREEN_H
