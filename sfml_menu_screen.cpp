#include "sfml_menu_screen.h"
#include "sfml_resources.h"
#include "sfml_button.h"
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
    m_main_text.setPosition(300, 100);

    sf::RectangleShape &b1_s = m_button1.get_shape();
    b1_s.setSize(sf::Vector2f(50, 50));
    b1_s.setPosition(sf::Vector2f(300, 300));
    b1_s.setFillColor(sf::Color(125, 5, 0));
}

void sfml_menu_screen::exec()
{
  while(m_window.isOpen())
  {
    static int i = 0;
    sf::Event event;
    if (m_button1.is_clicked(event, m_window)) {
      m_window.clear(sf::Color(0, 250, 255));
      i = 250;
    } else if (i > 0) {
      m_window.clear(sf::Color(0, i, i + 5));
      i--;
    } else {
      m_window.clear(sf::Color(255, 0, 0));
    }
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
    m_window.draw(m_main_text);
    m_window.draw(m_button1.get_shape());
    m_window.display();
  }
}
