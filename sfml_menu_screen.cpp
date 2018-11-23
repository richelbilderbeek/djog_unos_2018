#include "sfml_menu_screen.h"
#include "sfml_resources.h"
#include <iostream>

sfml_menu_screen::sfml_menu_screen()
    : m_window{sf::VideoMode(600, 600), "Nature Zen - Menu"},
      m_font{ sfml_resources::get().get_default_font() }
{
    m_main_text.setFont(m_font);
    m_main_text.setString("MAIN MENU");
    m_main_text.setOrigin(m_main_text.getGlobalBounds().left
        + m_main_text.getGlobalBounds().width / 2.0f,
      m_main_text.getGlobalBounds().top
        + m_main_text.getGlobalBounds().height / 2.0f);
    m_main_text.setPosition(300, 300);

    m_bg_sprite.setTexture(sfml_resources::get().get_gras_texture());
    m_bg_sprite.setScale(3,3);
}

void sfml_menu_screen::exec()
{
    while(m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            switch (event.type) //!OCLINT too few branches, please fix
            {
                case sf::Event::Closed:
                    m_window.close();
                    break;
                default:
                    break;
            }
        }
        m_window.clear();
        m_window.draw(m_bg_sprite);
        m_window.draw(m_main_text);
        m_window.display();
    }
}
