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
}

void sfml_menu_screen::exec()
{
  while(m_window.isOpen())
  {
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
    m_window.draw(m_main_text);
    m_window.display();
  }
}

sf::Color sfml_menu_screen::get_bg_color() //!OCLINT high ncss method
{
  process_red();
  process_green();
  process_blue();
  return sf::Color(r,g,b);
}

void sfml_menu_screen::process_red() {
  if (r < 200 && rb)
  {
    r++;
  }
  else if (r == 200 && rb)
  {
    rb = false;
  }
  if (r > 55 && !rb)
  {
    r--;
  }
  else if (r == 55 && !rb)
  {
    rb = true;
  }
}

void sfml_menu_screen::process_green() {
  if (g < 50 && gb)
  {
    g++;
  }
  else if (g == 50 && gb)
  {
    gb = false;
  }
  if (g > 0 && !gb)
  {
    g--;
  }
  else if (g == 0 && !gb)
  {
    gb = true;
  }
}

void sfml_menu_screen::process_blue() {
  if (b < 50 && bb)
  {
    b++;
  }
  else if (b == 50 && bb)
  {
    bb = false;
  }
  if (b > 0 && !bb)
  {
    b--;
  }
  else if (b == 0 && !bb)
  {
    bb = true;
  }
}
