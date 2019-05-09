#include "angle.h"
#include <cmath>

angle::angle()
{

}



void test_angle()
{
  const double x = 123.456;
  const angle a(x);
  assert(a.get_angle() == x);
}
