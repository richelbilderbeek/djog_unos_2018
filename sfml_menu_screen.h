#ifndef SFML_MENU_SCREEN_H
#define SFML_MENU_SCREEN_H

#include "sfml_button.h"
#include <SFML/Graphics.hpp>

class sfml_menu_screen
{
public:
    sfml_menu_screen();
    void exec();
private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    sf::Text m_main_text;
    sfml_button m_button1;
};

#endif // SFML_MENU_SCREEN_H
