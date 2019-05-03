#include "angle.h"
#include "game.h"
#include "agent_type.h"
#include "tile.h"
#include "tile_id.h"
#include "tile_type.h"


angle::angle()
{

}

void test_angle()
{
  const double x = 123.456;
  const angle a(x);
  assert(a.get_angle() == x);
}
