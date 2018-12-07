#ifndef TILE_ID_H
#define TILE_ID_H


class tile_id
{
public:
    tile_id(int id) { m_id = id; }
    void set(int id) { m_id = id; }
    int get() const noexcept { return m_id; }
private:
    int m_id;
};

#endif // TILE_ID_H
