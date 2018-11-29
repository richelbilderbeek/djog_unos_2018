#ifndef SFML_MENU_SCREEN_H
#define SFML_MENU_SCREEN_H
#include <SFML/Graphics.hpp>

class sfml_menu_screen
{
public:
    sfml_menu_screen();
    void exec();
    sf::Color get_bg_color();
    void process_red();
    void process_green();
    void process_blue();
private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    int r = 181;
    bool rb = true;
    int g = 3;
    bool gb = false;
    int b = 3;
    bool bb = true;
    sf::Text m_main_text;
};

#endif // SFML_MENU_SCREEN_H
