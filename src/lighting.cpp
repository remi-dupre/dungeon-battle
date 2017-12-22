#include "lighting.hpp"

float from_x_to_y(float a,float b,float c,float d,float x){
    return (x-c)*(b-d)/(a-c) + d;
}

float from_y_to_x(float a,float b,float c,float d,float y){
    return (y-d)*(a-c)/(b-d) + c;
}

bool can_be_seen(sf::Vector2i pos1, sf::Vector2i pos2, const Map& map){
    int pos_min_x = std::min(pos1.x,pos2.x);
    int pos_max_x = std::max(pos1.x,pos2.x);
    int pos_min_y = std::min(pos1.y,pos2.y);
    int pos_max_y = std::max(pos1.y,pos2.y);


    if (pos1.x == pos2.x){
        int last_y = pos_min_y;
        for(int j = pos_min_y; j <= pos_max_y; j++){
            int i = pos1.x;
            if (map.cellAt(i,j) != CellType::Floor) break;
            last_y = j;
        }
        return pos_max_y ==last_y;
    }
    if (pos1.y == pos2.y){
        int last_x = pos_min_x;
        for(int i = pos_min_x; i <= pos_max_x; i++){
            int j = pos1.y;
            if (map.cellAt(i,j) != CellType::Floor) break;
            last_x = i;
        }
        return pos_max_x == last_x;
    }

    auto f_x = [pos1,pos2](float x){
    return from_x_to_y((float) pos1.x + 0.5f, (float) pos1.y + 0.5f,
    (float) pos2.x + 0.5f, (float) pos2.y + 0.5f,x);
    };
    auto f_y = [pos1,pos2](float y){
    return from_y_to_x((float) pos1.x + 0.5f, (float) pos1.y + 0.5f,
    (float) pos2.x + 0.5f, (float) pos2.y + 0.5f,y);
    };


    int last_x = pos_min_x;

    for(int i = pos_min_x+1; i <= pos_max_x; i++){
        int j = std::floor(f_x(i));
        if (map.cellAt(i,j) != CellType::Floor) break;
        last_x = i;
    }

    int last_y = pos_min_y;

    for(int j = pos_min_y+1; j <= pos_max_y; j++){
        int i = std::floor(f_y(j));
        if (map.cellAt(i,j) != CellType::Floor) break;
        last_y = j;
    }

    return (pos_max_y == last_y)&&(pos_max_x == last_x);
}
