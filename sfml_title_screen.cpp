#include "sfml_title_screen.h"
#include "sfml_resources.h"
#include <iostream>

sfml_title_screen::sfml_title_screen(const int close_at)
  : m_title_music{ sfml_resources::get().get_title_music() },
    m_window{VideoMode(800, 600), "Nature Zen - Title"},
    m_font{ sfml_resources::get().get_title_font() },
    m_close_at{close_at}
{
  m_title_music.setLoop(true);
  m_title_music.play();
  title_text.setFont(m_font);
  title_text.setString("Nature Zen");
  title_text.setOrigin(title_text.getGlobalBounds().left
      + title_text.getGlobalBounds().width / 2.0f,
    title_text.getGlobalBounds().top
      + title_text.getGlobalBounds().height / 2.0f);
  title_text.setScale(2,2);
  #if(SFML_VERSION_MINOR > 1)
  title_text.setOutlineColor(Color(36, 211, 16));
  title_text.setFillColor(Color(155, 40, 0));
  title_text.setOutlineThickness(1);
  #else
  //Only relevant for Travis
  title_text.setColor(Color(36, 211, 16));
  #endif
  title_text.setPosition(400, 200);

  m_bg_sprite.setTexture(sfml_resources::get().get_background_image());
  m_bg_sprite.setOrigin(m_bg_sprite.getGlobalBounds().left
                        + m_bg_sprite.getGlobalBounds().width / 2.0f,
                        m_bg_sprite.getGlobalBounds().top
                        + m_bg_sprite.getGlobalBounds().height / 2.0f);
  m_bg_sprite.setPosition(400, 300);
  m_bg_sprite.setScale(4,4);
}

void sfml_title_screen::exec() //!OCLINT must be shorter
{
    if (m_close_at >= 0) m_window.close();
    while(m_window.isOpen()) {
        title_text.setPosition(400, 110+i);
        animation();
        Event event;
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
        m_window.clear();
        m_window.draw(m_bg_sprite);
        m_window.draw(title_text);
        m_window.display();
    }
}

void sfml_title_screen::stop_music() {
  m_title_music.stop();
}

void sfml_title_screen::animation() {
  if (i < 70.0 && b) {
    i += 0.05;
  } else if (b) {
    i -= 0.05;
    b = false;
  }
  if (i > 0.0 && !b) {
    i -= 0.05;
  } else if (!b) {
    i += 0.05;
    b = true;
  }
}

sfml_title_screen::~sfml_title_screen()
{
  stop_music();
}
