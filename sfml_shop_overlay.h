#ifndef SFML_OVERLAY_OVERLAY_H
#define SFML_OVERLAY_OVERLAY_H

#include "sfml_button.h"
#include "game_state.h"
#include "game.h"
#include "sfml_camera.h"
#include <SFML/Graphics.hpp>

class sfml_shop_overlay
{
public:
    sfml_shop_overlay();

    void exec(game& g, sfml_camera& m_camera);

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void set_positions();

    void draw_objects();

    std::string extract_ints(std::ctype_base::mask category, std::string str, std::ctype<char> const& facet);

    std::string extract_ints(std::string str);

    void button_clicked(sf::Text button, game& g);

    void place_on_grid(game& g);

private:
    sf::RenderWindow& m_window;

    sf::Font m_font;

    sf::Text m_header;

    sfml_button m_button1;

    sfml_button m_button2;

    sfml_button m_button3;

    sf::RectangleShape m_bg_rect;

    bool follow_tile = false;

    int m_price;
};
///position button2
sf::Vector2f get_grid_position(sfml_button & b, int x_p, int y_p);

///position button3
sf::Vector2f get_grid_position1(sfml_button & b, int x_p, int y_p);

#endif // SFML_OVERLAY_OVERLAY_H
