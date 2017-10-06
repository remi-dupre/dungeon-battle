#include "ai.hpp"


Action bfs_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    sf::Vector2i startposition = entity.getPosition();
    int sight = entity.getSightRadius();

    sf::Vector2i hero_postion;
    if (has_hero(entities)) hero_postion = get_hero_position(entities);
    else return just_moving(entity,map);

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
            Action ret(ActionType::Attack, dirtoact[ori]);
            return ret;
        }
        Action ret(ActionType::Move, dirtoact[ori]);
        next_cells.push(std::make_tuple(position,1,ret));
    }

}

Action get_input_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    assert(has_hero(entities));
    return just_moving(entity,map);
}

Action just_moving(const Character& entity, const Map& map)
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
