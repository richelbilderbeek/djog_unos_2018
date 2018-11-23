#include "sfml_menu_screen.h"
#include "sfml_resources.h"
#include <iostream>

sfml_menu_screen::sfml_menu_screen()
    : m_window{sf::VideoMode(600, 600), "Nature Zen - Menu"},
      m_font{ sfml_resources::get().get_default_font() }
{
    mainMenuScreenText.setFont(m_font);
    mainMenuScreenText.setString("MAIN MENU");
    mainMenuScreenText.setOrigin(mainMenuScreenText.getGlobalBounds().left
        + mainMenuScreenText.getGlobalBounds().width / 2.0f,
      mainMenuScreenText.getGlobalBounds().top
        + mainMenuScreenText.getGlobalBounds().height / 2.0f);
    mainMenuScreenText.setPosition(300, 300);

//    m_bg_sprite.setTexture(sfml_resources::get().get_gras_texture());
//    m_bg_sprite.setScale(3,3);
}

void sfml_menu_screen::exec()
{
    while(m_window.isOpen()) {
        m_window.clear(get_bg_color());
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
//        m_window.draw(m_bg_sprite);
        m_window.draw(mainMenuScreenText);
        m_window.display();
    }
}

sf::Color sfml_menu_screen::get_bg_color() {
  if (r < 255 && rb) {
    r++;
  } else if (r == 255 && rb) {
    rb = false;
  }
  if (r > 0 && !rb) {
    r--;
  } else if (r == 0 && !rb) {
    rb = true;
  }

  if (g < 255 && gb) {
    g++;
  } else if (g == 255 && gb) {
    gb = false;
  }
  if (g > 0 && !gb) {
    g--;
  } else if (g == 0 && !gb) {
    gb = true;
  }

  if (b < 255 && bb) {
    b++;
  } else if (b == 255 && bb) {
    bb = false;
  }
  if (b > 0 && !bb) {
    b--;
  } else if (b == 0 && !bb) {
    bb = true;
  }

  return sf::Color(r,g,b);
}
