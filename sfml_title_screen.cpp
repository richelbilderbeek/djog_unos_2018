#include "sfml_title_screen.h"
#include "sfml_resources.h"
#include <iostream>

sfml_title_screen::sfml_title_screen()
  : m_window{sf::VideoMode(600, 600), "Nature Zen - Title"},
    m_font{ sfml_resources::get().get_title_font() }
{
  title_text.setFont(m_font);
  title_text.setString("Nature Zen");
  title_text.setOrigin(title_text.getGlobalBounds().left
      + title_text.getGlobalBounds().width / 2.0f,
    title_text.getGlobalBounds().top
      + title_text.getGlobalBounds().height / 2.0f);
  title_text.setScale(2,2);
  title_text.setOutlineColor(sf::Color(0, 180, 30));
  title_text.setFillColor(sf::Color(155, 40, 0));
  title_text.setOutlineThickness(1);
  title_text.setPosition(300, 200);

  m_bg_sprite.setTexture(sfml_resources::get().get_bacterie_texture());
  m_bg_sprite.setOrigin(m_bg_sprite.getGlobalBounds().left
                        + m_bg_sprite.getGlobalBounds().width / 2.0f,
                        m_bg_sprite.getGlobalBounds().top
                        + m_bg_sprite.getGlobalBounds().height / 2.0f);
  m_bg_sprite.setPosition(300, 300);
  m_bg_sprite.setScale(30,30);
  m_bg_sprite.setRotation(90);
}

void sfml_title_screen::exec()
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
        m_window.draw(title_text);
        m_window.display();
    }
}
