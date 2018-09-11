#include "game.h";
#include <boost/test/unit_test.hpp>;

BOOST_AUTO_TEST_CASE(game_default_construct)
{
  game g;
  BOOST_CHECK(g.get_n_displayed_max() == 100); //STUB
  const int exit_code{g.exec()};
  BOOST_CHECK(g.get_n_displayed() == 100); //STUB
  BOOST_CHECK(exit_code == 0);
}

