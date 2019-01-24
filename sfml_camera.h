#ifndef SFML_CAMERA_H
#define SFML_CAMERA_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class sfml_camera
{
public:
  sfml_camera();

  double x{-100.0};
  double y{-100.0};
  void move_camera(sf::Vector2f offset);

  bool m_movecam_r = false;
  bool m_movecam_l = false;
  bool m_movecam_u = false;
  bool m_movecam_d = false;
  bool m_block_movement = false;

private:
  sf::RenderWindow& m_window;
};

#endif // SFML_CAMERA_H
