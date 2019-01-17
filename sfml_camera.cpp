#include "sfml_camera.h"

void sfml_camera::move_camera(sf::Vector2f offset)
{
  // Dont move the camera in the menu
  if (m_block_movement)
    return;
  x += offset.x;
  y += offset.y;
}
