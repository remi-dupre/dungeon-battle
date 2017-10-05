#include "ai.hpp"

bool has_hero(const std::vector<std::shared_ptr<Entity>>& entities){
    for(const auto& entity : entities)
    {
        if(entity->getType() == EntityType::Hero)
        {
            return true;
        }
    }
    return false;
}


sf::Vector2u get_hero_position(const std::vector<std::shared_ptr<Entity>>& entities){
    for(const auto& entity : entities)
    {
        if(entity->getType() == EntityType::Hero)
        {
            return entity->getPosition();
        }
    }

    return {0, 0};
}

/*
sf::Vector2u relative_position(int x, int y, sf::Vector2u position){
return
}*/

Action get_input_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    sf::Vector2u position = entity.getPosition();
    int sight = entity.getSightRadius();

    sf::Vector2u hero_postion;
    if (has_hero(entities)) hero_postion = get_hero_position(entities);
    else return just_moving(entity,entities,map);

    int sightperimeter = 2*sight+1;
    std::vector<std::vector<bool>> seen(sightperimeter, std::vector<bool>(sightperimeter,false) );

    int curent_x = 0;
    int curent_y = 0;

    return Action();
}

Action just_moving(const Character& entity, [[maybe_unused]] const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    sf::Vector2u position = entity.getPosition();

    if (map.cellAt(position.x-1, position.y) == CellType::Floor)
        return Action(ActionType::Move, Direction::Left);
    if (map.cellAt(position.x+1, position.y) == CellType::Floor)
        return Action(ActionType::Move, Direction::Right);
    if (map.cellAt(position.x, position.y-1) == CellType::Floor)
        return Action(ActionType::Move, Direction::Up);
    if (map.cellAt(position.x, position.y+1) == CellType::Floor)
        return Action(ActionType::Move, Direction::Down);

    return Action();
}
