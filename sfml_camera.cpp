#include "sfml_camera.h"
#include "sfml_window_manager.h"
using namespace sf;


sfml_camera::sfml_camera()
  :m_window{ sfml_window_manager::get().get_window() }
{
  sf::View view = m_window.getDefaultView();
  view.setCenter(m_window.mapPixelToCoords(sf::Vector2i(x, y)));
  m_window.setView(view);
}

void sfml_camera::move_camera(Vector2f offset)
{
  // Dont move the camera in the menu
  if (m_block_movement)
    return;
  sf::View view = m_window.getView();
  x += offset.x;
  y += offset.y;
  view.setCenter(sf::Vector2f(x, y));
  m_window.setView(view);
  sfml_window_manager::get().update();
}
