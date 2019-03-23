#include "sfml_zen_bar.h"
#include "sfml_resources.h"

sfml_zen_bar::sfml_zen_bar()
{
    m_zen_bar.setSize(sf::Vector2f(sfml_resources::get().get_zen_bar().getSize()));
    m_zen_bar.setTexture(&sfml_resources::get().get_zen_bar());

    m_zen_icon.setSize(sf::Vector2f(sfml_resources::get().get_zen_ind().getSize()));
    m_zen_icon.setTexture(&sfml_resources::get().get_zen_ind());

}

sf::RectangleShape sfml_zen_bar::get_drawable_bar(
  float x_pos,
  float y_pos,
  sf::RenderWindow &window
)
{
    m_zen_bar.setPosition(sf::Vector2f(
                            (x_pos - (m_zen_bar.getSize().x/2.0f)),
                            y_pos));
    m_zen_bar.setPosition(window.mapPixelToCoords(sf::Vector2i(m_zen_bar.getPosition())));

    return m_zen_bar;
}

sf::RectangleShape sfml_zen_bar::get_drawable_ind(
  float x_pos,
  float y_pos,
  sf::RenderWindow &window
)
{
  m_zen_icon.setPosition(
    sf::Vector2f(
      x_pos - (m_zen_icon.getSize().x/2.0f) + m_zen_balance,
      y_pos + (m_zen_bar.getSize().y/2.0f) - (m_zen_icon.getSize().x/2.0f)
    )
  );
  m_zen_icon.setPosition(
    window.mapPixelToCoords(sf::Vector2i(m_zen_icon.getPosition()))
  );
  return m_zen_icon;
}

void sfml_zen_bar::set_score(float score)
{
    m_zen_balance = score;
}
