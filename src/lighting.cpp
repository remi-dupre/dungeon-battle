#include "lighting.hpp"


float from_x_to_y(float a,float b,float c,float d,float x){
    return (x-c)*(b-d)/(a-c) + d;
}

float from_y_to_x(float a,float b,float c,float d,float y){
    return (y-d)*(a-c)/(b-d) + c;
}

bool raytrace(sf::Vector2i pos1, sf::Vector2i pos2, const Map& map)
{
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


bool can_be_seen(sf::Vector2i pos1, sf::Vector2i pos2, const Map& map)
{
    if (pos1 == pos2)
        return true;

    return raytrace(pos1, pos2, map);

    // int pos_min_x = std::min(pos1.x, pos2.x);
    // int pos_max_x = std::max(pos1.x, pos2.x);
    // int pos_min_y = std::min(pos1.y, pos2.y);
    // int pos_max_y = std::max(pos1.y, pos2.y);

    // if (pos1.x == pos2.x)
    // {
    //     int last_y = pos_min_y;
    //     for(int j = pos_min_y + 1; j < pos_max_y; j++)
    //     {
    //         if (map.cellAt(pos1.x,j) != CellType::Floor)
    //             break;
    //         last_y = j;
    //     }
    //     return pos_max_y == last_y + 1;
    // }

    // if (pos1.y == pos2.y)
    // {
    //     int last_x = pos_min_x;
    //     for(int i = pos_min_x + 1; i < pos_max_x; i++)
    //     {
    //         if (map.cellAt(i,pos1.y) != CellType::Floor)
    //             break;
    //         last_x = i;
    //     }
    //     return pos_max_x == last_x + 1;
    // }

    // auto f_x = [pos1,pos2](float x)
    //     {
    //         return from_x_to_y((float) pos1.x + 0.5f, (float) pos1.y + 0.5f,
    //                            (float) pos2.x + 0.5f, (float) pos2.y + 0.5f, x);
    //     };

    // auto f_y = [pos1,pos2](float y)
    //     {
    //         return from_y_to_x((float) pos1.x + 0.5f, (float) pos1.y + 0.5f,
    //                            (float) pos2.x + 0.5f, (float) pos2.y + 0.5f, y);
    //     };


    // int last_x = pos_min_x;
    // for(int i = pos_min_x+1; i <= pos_max_x; i++)
    // {
    //     int j = std::floor(f_x(i));
    //     if (map.cellAt(i,j) != CellType::Floor)
    //         break;
    //     last_x = i;
    // }

    // int last_y = pos_min_y;
    // for(int j = pos_min_y+1; j <= pos_max_y; j++)
    // {
    //     int i = std::floor(f_y(j));
    //     if (map.cellAt(i,j) != CellType::Floor)
    //         break;
    //     last_y = j;
    // }

    // return (pos_max_y == last_y) && (pos_max_x == last_x);
}
