#include "sfml_menu_screen.h"
#include "sfml_resources.h"
#include "sfml_button.h"
#include <iostream>
#include <cassert>

sfml_menu_screen::sfml_menu_screen(const int close_at)
    : m_window{VideoMode(600, 600), "Nature Zen - Menu"},
      m_font{ sfml_resources::get().get_default_font() },
      m_close_at{close_at}
{
    m_main_text.setFont(m_font);
    m_main_text.setString("MAIN MENU");
    m_main_text.setOrigin(m_main_text.getGlobalBounds().left
        + m_main_text.getGlobalBounds().width / 2.0f,
      m_main_text.getGlobalBounds().top
        + m_main_text.getGlobalBounds().height / 2.0f);
    m_main_text.setPosition(300, 100);

    RectangleShape &b1_s = m_button1.get_shape();
    b1_s.setFillColor(Color(125, 5, 0));
    m_button1.set_size(250, 100);
    m_button1.set_pos(300, 300);
}

void sfml_menu_screen::exec()
{
  if (m_close_at >= 0) m_window.close();
  while(m_window.isOpen())
  {
    static int i = 0;
    Event event;
    if (m_button1.is_clicked(event, m_window)) {
      m_window.clear(Color(0, 250, 255));
      i = 250;
    } else if (i > 0) {
      m_window.clear(Color(0, i, i + 5));
      i--;
    } else {
      m_window.clear(Color(255, 0, 0));
    }
    while (m_window.pollEvent(event))
    {
      switch (event.type) //!OCLINT too few branches, please fix
      {
        case Event::Closed:
            m_window.close();
            break;
        default:
            break;
      }
    }
    m_window.draw(m_main_text);
    m_window.draw(m_button1.get_shape());
    m_window.draw(m_button1.get_text());
    m_window.display();
  }
}
