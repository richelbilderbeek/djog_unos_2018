#include "biology.h"
#include <cassert>

biology::biology()
{
  //Nothing to be done yet :-)
}

double biology::get_health_increase_when_eating() const noexcept
{
  return 0.2;
}

double biology::get_stamina_increase_when_eating() const noexcept
{
  return 0.2;
}

double biology::get_health_decrease_when_eaten() const noexcept
{
  return 2.0;
}

double biology::get_grass_reproduction_health() const noexcept
{
  return 100.0;
}

void test_biology()
{

  #define FIX_ISSUE_549_A
  #ifdef FIX_ISSUE_549_A
  {
    const biology b;
    assert(b.get_health_increase_when_eating() == 0.2);
  }
  #endif // FIX_ISSUE_549_A

  #define FIX_ISSUE_549_B
  #ifdef FIX_ISSUE_549_B
  {
    const biology b;
    assert(b.get_stamina_increase_when_eating() == 0.2);
  }
  #endif // FIX_ISSUE_549_B

  #define FIX_ISSUE_549_C
  #ifdef FIX_ISSUE_549_C
  {
    const biology b;
    assert(b.get_health_decrease_when_eaten() == 2.0);
  }
  #endif // FIX_ISSUE_549_C
  {
    const biology b;
    assert(b.get_grass_reproduction_health() == 100.0);
  }
}
