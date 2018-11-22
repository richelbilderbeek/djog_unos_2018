#include "id.h"

int new_id() noexcept
{
  static int old_id = 0; //!OCLINT accepted idiom to use static here
  ++old_id;
  return old_id;
}
