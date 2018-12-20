#include "tile_id.h"

tile_id::tile_id() {
    ++old_id;
    m_id = old_id;
}
int tile_id::get_new_tile_id()
{
  ++old_id;
  return old_id;
}

int tile_id::get_old_tile_id()
{
  return old_id;
}
void test_tile_id()
{
    //Tile IDs must be unique
    const tile_id a;
    const tile_id b;
    assert(a.get() != b.get());

    //Tile IDs can be copied
    tile_id c;
    tile_id d;
    c = d;
    assert(c.get() == d.get());
}
