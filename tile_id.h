#ifndef TILE_ID_H
#define TILE_ID_H
#include "id.h"
#include <cassert>

static int old_id = 0;

class tile_id
{
public:
    tile_id();

    int get() const noexcept { return m_id; }

    static int get_new_tile_id();

    static int get_old_tile_id();
private:

    int m_id;

};

void test_tile_id();

#endif // TILE_ID_H
