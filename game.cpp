//Always include the header of the unit first
#include "game.h"

#include <cassert>

game::game()
  : m_tiles{}
{
  //Add first tile
  {
    tile t(200, 200, 400, 200, tile_type::grassland);
    m_tiles.push_back(t);
  }
}

void test_game() //!OCLINT a testing function may be long
{
  //A game starts with one or more tiles
  {
    const game g;
    assert(!g.get_tiles().empty());
  }
  //#define FIX_ISSUE_89_ADD_SECOND_TILE
  #ifdef FIX_ISSUE_89_ADD_SECOND_TILE
  {
    const game g;
    assert(!g.get_tiles().size() >= 2);
    assert(g.get_tiles()[0].get_type() != g.get_tiles()[1].get_type());
  }
  #endif // FIX_ISSUE_89_ADD_SECOND_TILE

  //#define FIX_ISSUE_90_GAME_MUST_HAVE_A_SCORE
  #ifdef FIX_ISSUE_90_GAME_MUST_HAVE_A_SCORE
  //A game starts with a score of zero
  {
    const game g;
    assert(g.get_score() == 0);
  }
  #endif // FIX_ISSUE_90_GAME_MUST_HAVE_A_SCORE

  //#define FIX_ISSUE_91_GAME_TRACKS_THE_NUMBER_OF_TICKS
  #ifdef FIX_ISSUE_91_GAME_TRACKS_THE_NUMBER_OF_TICKS
  //A game starts with a zero number of game cycles
  {
    const game g;
    assert(g.get_n_ticks() == 0);
  }
  //Number of game cycles is increased each time all events are processed
  {
    game g;
    g.process_events();
    assert(g.get_n_ticks() == 1);
  }
  #endif // FIX_ISSUE_91_GAME_TRACKS_THE_NUMBER_OF_TICKS


  //#define FIX_ISSUE_95_GAME_CAN_BE_SAVED
  #ifdef FIX_ISSUE_95_GAME_CAN_BE_SAVED
  //A game can be saved
  {
    const game g;
    const std::string filename{"tmp.sav"};
    if (file_exists(filename)) delete_file(filename);
    assert(!file_exists(filename));
    save(g, filename);
    assert(file_exists(filename));
  }
  #endif // FIX_ISSUE_95_GAME_CAN_BE_SAVED
}
