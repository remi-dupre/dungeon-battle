#include "ai.hpp"

bool has_hero(const std::vector<std::shared_ptr<Entity>>& entities)
{
    for(const auto& entity : entities)
    {
        if(entity->getType() == EntityType::Hero)
        {
            return true;
        }
    }
    return false;
}


sf::Vector2i get_hero_position(const std::vector<std::shared_ptr<Entity>>& entities)
{
    for(const auto& entity : entities)
    {
        if(entity->getType() == EntityType::Hero)
        {
            return entity->getPosition();
        }
    }

    return {0, 0};
}


bool cell_seen(std::vector<std::vector<bool>> seen, sf::Vector2i cell, sf::Vector2i startcell, int sight)
{
    return seen[cell.x+sight-startcell.x][cell.y+sight-startcell.y];
}


Action bfs_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    sf::Vector2i startposition = entity.getPosition();
    int sight = entity.getSightRadius();

    sf::Vector2i hero_postion;
    if (has_hero(entities)) hero_postion = get_hero_position(entities);
    else return just_moving(entity,entities,map);

    int sightperimeter = 2*sight+1;
    std::vector<std::vector<bool>> seen(sightperimeter, std::vector<bool>(sightperimeter,false) );

    sf::Vector2i curentposition = startposition;

    std::queue<std::tuple<sf::Vector2i,int,Action>> next_cells;

    std::vector<sf::Vector2i> dir ={{1,0},{-1,0},{0,1},{0,-1}};
    std::map<sf::Vector2i,Direction> dirtoact = {
        {{1,0} , Direction::Left},
        {{-1,0}, Direction::Right},
        {{0,1} , Direction::Up},
        {{0,-1}, Direction::Down}};

    for(auto ori : dir)
    {
        sf::Vector2i position = curentposition + ori;
        if (position == hero_postion){
            return Action(ActionType::Attack, std::map(ori));
        }
        next_cells.push(std::make_tuple(position,1,Action(ActionType::Move, std::map(ori))))
    }

}

Action get_input_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    return just_moving(entity,entities,map);
}

Action just_moving(const Character& entity, [[maybe_unused]] const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
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
