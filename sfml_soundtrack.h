#ifndef SFML_SOUNDTRACK_H
#define SFML_SOUNDTRACK_H

#include <cassert>

#include <SFML/Audio.hpp>

class sfml_soundtrack
{
public:
  sfml_soundtrack();
private:
  sf::Music m_music;

  const bool m_loop;

  bool m_playing;


};

#endif // SFML_SOUNDTRACK_H
