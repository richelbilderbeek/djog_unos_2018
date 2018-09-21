#ifndef GAME_H
#define GAME_H

#include <vector>

#include "tile.h"

class game
{

public:
  ///Constructor
  game();

  ///Read all tiles
  const auto& get_tiles() const noexcept { return m_tiles; }

private:

  ///Timer, physics, bullets moving, etc.
  ///Everything except user input.
  void process_events();

  std::vector<tile> m_tiles;
};

#endif // GAME_H
