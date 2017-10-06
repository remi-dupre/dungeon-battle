#include "ai.hpp"


Action BFS_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    sf::Vector2i position = entity.getPosition();
    int sight = entity.getSightRadius();

    sf::Vector2i hero_postion;
    if (has_hero(entities)) hero_postion = get_hero_position(entities);
    else return just_moving(entity,entities,map);

    int sightperimeter = 2*sight+1;
    std::vector<std::vector<bool>> seen(sightperimeter, std::vector<bool>(sightperimeter,false) );

    int curent_x = 0;
    int curent_y = 0;
}

Action get_input_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{

    return just_moving(entity,entities,map);
}

Action just_moving(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    sf::Vector2i position = entity.getPosition();

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
