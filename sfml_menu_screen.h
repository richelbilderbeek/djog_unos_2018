#ifndef SFML_MENU_SCREEN_H
#define SFML_MENU_SCREEN_H
#include <SFML/Graphics.hpp>

class sfml_menu_screen
{
public:
    sfml_menu_screen();
    void exec();
    sf::Color get_bg_color();
private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    sf::Text mainMenuScreenText;
    sf::Sprite m_bg_sprite;
    int r = 181;
    bool rb = true;
    int g = 3;
    bool gb = false;
    int b = 3;
    bool bb = true;
};

#endif // SFML_MENU_SCREEN_H
