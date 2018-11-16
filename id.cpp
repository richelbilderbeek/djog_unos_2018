#include "id.h"

int new_id() noexcept
{
  static int old_id = 0; //!OCLINT use static here
  ++old_id;
  return old_id;
}
