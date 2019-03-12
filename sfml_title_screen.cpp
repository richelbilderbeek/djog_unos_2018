#include "sfml_title_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include "sfml_button.h"
#include <cassert>
#include <iostream>

sfml_title_screen::sfml_title_screen(const int close_at)
  : m_title_music{ sfml_resources::get().get_title_music() },
    m_window{ sfml_window_manager::get().get_window() },
    m_font{ sfml_resources::get().get_title_font() },
    m_default_font{ sfml_resources::get().get_default_font() },
    m_close_at{close_at}
{
  m_title_music.setLoop(true);
  m_title_music.play();
  title_text.setFont(m_font);
  title_text.setString("NATURE ZEN");
  title_text.setOrigin(title_text.getGlobalBounds().left
      + title_text.getGlobalBounds().width / 2.0f,
    title_text.getGlobalBounds().top
      + title_text.getGlobalBounds().height / 4.0f);
  title_text.setScale(3,3);
  #if(SFML_VERSION_MINOR > 3)
  title_text.setOutlineColor(sf::Color(36, 211, 16));
  title_text.setFillColor(sf::Color(155, 40, 0));
  title_text.setOutlineThickness(1);
  #else
  //Only relevant for Travis and RuG
  title_text.setColor(sf::Color(36, 211, 16));
  #endif

  copyright_text.setFont(m_default_font);
  copyright_text.setString("(C) 2018 Team Octane");
  #if(SFML_VERSION_MINOR > 3)
  copyright_text.setOutlineColor(sf::Color::Black);
  copyright_text.setFillColor(sf::Color::Black);
  #else
  //Only relevant for Travis and RuG
  copyright_text.setColor(sf::Color::Black);
  #endif
  copyright_text.setPosition(m_window.getSize().x / 100, m_window.getSize().y / 1.05f);

  m_zen_title.setTexture(sfml_resources::get().get_zen_title());
  m_zen_title.setScale(2,2);
  m_zen_title.setOrigin(m_zen_title.getGlobalBounds().left
                        + m_zen_title.getGlobalBounds().width / 2.0f,
                      m_zen_title.getGlobalBounds().top
                        + m_zen_title.getGlobalBounds().height / 2.0f);
  m_zen_title.setPosition(m_window.getSize().x / 1.75f, m_window.getSize().y / 1.5f);

  m_bg_sprite.setTexture(sfml_resources::get().get_background_image());
  stretch_bg();
}

void sfml_title_screen::exec() //!OCLINT must be shorter
{
  while(active(game_state::titlescreen)) {
    if (m_close_at >= 0) close(game_state::menuscreen);
    animation();
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      sf::View view = m_window.getDefaultView();
      switch (event.type) //!OCLINT TODO too few branches, please fix
      {
        case sf::Event::Closed:
          close();
          break;
        case sf::Event::Resized:
          sfml_window_manager::get().update();
          view.setSize(static_cast<float>(m_window.getSize().x),
                       static_cast<float>(m_window.getSize().y));
          m_window.setView(view);
          stretch_bg();
          break;
        case sf::Event::KeyPressed:
            sfml_window_manager::get().set_state(game_state::menuscreen);
            break;
        case sf::Event::MouseButtonPressed:
            sfml_window_manager::get().set_state(game_state::menuscreen);
            break;
        default:
          sfml_window_manager::get().process();
          break;
      }
    }
    title_text.setPosition(m_window.getSize().x/2,
                           m_window.getView().getCenter().y-(m_window.getSize().y/2)+
                           (m_window.getSize().y/568)*130+i);
    title_text.setPosition(m_window.mapPixelToCoords(
                           sf::Vector2i(title_text.getPosition())));

    //m_bg_sprite.setPosition(sf::Vector2f(m_window.getView().getCenter().x -
    //                                     m_bg_sprite.getTexture()->getSize().x,
    //                                     m_window.getView().getCenter().y -
    //                                     m_bg_sprite.getTexture()->getSize().y));
    m_bg_sprite.setPosition(0, 0);
    m_bg_sprite.setPosition(m_window.mapPixelToCoords(
                               sf::Vector2i(m_bg_sprite.getPosition())));

    m_window.clear();
    m_window.draw(m_bg_sprite);
    m_window.draw(title_text);
    m_window.draw(copyright_text);
    m_window.draw(m_zen_title);
    m_window.display();
  }
}

void sfml_title_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_title_screen::close() {
  m_window.close();
}

void sfml_title_screen::stop_music() {
  m_title_music.stop();
}

void sfml_title_screen::stretch_bg() {
  sf::Vector2f size = sf::Vector2f(m_bg_sprite.getTexture()->getSize());
  float scale_x = m_window.getSize().x/size.x;
  float scale_y = m_window.getSize().y/size.y;
  m_bg_sprite.setScale(scale_x, scale_y);
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
