#ifndef SFML_WINDOW_MANAGER_H
#define SFML_WINDOW_MANAGER_H

#include <SFML/Graphics.hpp>

class sfml_window_manager
{
public:

  sfml_window_manager& get();

private:

  sfml_window_manager();

  static sfml_window_manager *m_instance;

  sf::RenderWindow m_window;

};

#endif // SFML_WINDOW_MANAGER_H
