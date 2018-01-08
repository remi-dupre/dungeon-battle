#include "lighting.hpp"


bool can_be_seen(sf::Vector2i pos1, sf::Vector2i pos2, const Map& map)
{
    if (pos1 == pos2)
        return true;

    int dx = std::abs(pos1.x - pos2.x);
    int dy = std::abs(pos1.y - pos2.y);

    int x = pos1.x;
    int y = pos1.y;

    int x_inc = (pos1.x < pos2.x) ? 1 : -1;
    int y_inc = (pos1.y < pos2.y) ? 1 : -1;

    int shift = dx - dy;

    for (int n = 0; n < dx + dy; ++n)
    {
        if (map.cellAt(x, y) != CellType::Floor)
            return false;

        if (shift == 0)
        {
            if (map.cellAt(x + x_inc, y) != CellType::Floor && map.cellAt(x, y + y_inc) != CellType::Floor)
                return false;
            x += x_inc;
            y += y_inc;
            shift += 2 * dx - 2 * dy;
            ++n;
        }
        else if (shift > 0)
        {
            x += x_inc;
            shift -= 2 * dy;
        }
        else
        {
            y += y_inc;
            shift += 2 * dx;
        }
    }

    return true;
}
