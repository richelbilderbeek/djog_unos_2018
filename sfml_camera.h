#ifndef SFML_CAMERA_H
#define SFML_CAMERA_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;

class sfml_camera
{
public:
    double x{-100.0};
    double y{-100.0};
    void move_camera(sf::Vector2f offset);

    bool movecam_r = false;
    bool movecam_l = false;
    bool movecam_u = false;
    bool movecam_d = false;
    bool block_movement = false;

};

#endif // SFML_CAMERA_H
