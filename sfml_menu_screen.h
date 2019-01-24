#ifndef SFML_MENU_SCREEN_H
#define SFML_MENU_SCREEN_H

#include "sfml_button.h"
#include <SFML/Graphics.hpp>

class sfml_menu_screen
{
public:
    sfml_menu_screen(const int close_at);
    void exec();
private:
    RenderWindow m_window;
    Font m_font;
    Text m_main_text;
    sfml_button m_button1;

    int m_close_at;
};

#endif // SFML_MENU_SCREEN_H
