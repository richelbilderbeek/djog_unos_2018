#include "game.h"
#include <QFile>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(game_default_construct)
{
  { QFile f(":/nature_zen/resources/background_music.ogg"); f.copy("background_music.ogg"); }
  game g;
  BOOST_CHECK(g.get_n_displayed_max() == 100); //STUB
  const int exit_code{g.exec()};
  BOOST_CHECK(g.get_n_displayed() == 100); //STUB
  BOOST_CHECK(exit_code == 0);
}

