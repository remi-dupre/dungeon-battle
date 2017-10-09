#include "ai.hpp"


bool cell_seen(std::vector<std::vector<bool>> &seen, sf::Vector2i position, sf::Vector2i startposition, int sight){
    bool test = seen[position.x + sight - startposition.x][position.y + sight - startposition.y];
    seen[position.x + sight - startposition.x][position.y + sight - startposition.y] = true;
    return test;
}


void thereisaobstacle(
    const std::vector<std::shared_ptr<Entity>>& entities,
    std::vector<std::vector<bool>> &seen,
    sf::Vector2i startposition,
    int sight)
{
    for(std::shared_ptr<Entity> entity : entities)
    {
        sf::Vector2i position = entity->getPosition();
        if((std::abs(position.x - startposition.x) <= sight)
            && (std::abs(position.y-startposition.y)<= sight)){
            cell_seen(seen, position, startposition,sight);
        }
    }
}


Action bfs_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    sf::Vector2i startposition = entity.getPosition();
    int sight = 6; //entity.getSightRadius();

    sf::Vector2i hero_postion;
    if (has_hero(entities)) hero_postion = get_hero_position(entities);
    else return just_moving(entity,map);

    int sightperimeter = 2*sight+1;
    std::vector<std::vector<bool>> seen(sightperimeter, std::vector<bool>(sightperimeter,false) );

    sf::Vector2i curentposition = startposition;

    thereisaobstacle(entities,seen,startposition,sight);

    std::queue<std::tuple<sf::Vector2i,int,Action>> next_cells;

    std::vector<sf::Vector2i> dir ={{1,0},{-1,0},{0,1},{0,-1}};
    std::map<sf::Vector2i,Direction> dirtoact = {
        {{-1,0} , Direction::Left},
        {{1,0}, Direction::Right},
        {{0,-1} , Direction::Up},
        {{0,1}, Direction::Down}};


    for(auto ori : dir)
    {
        sf::Vector2i position = curentposition + ori;
        if (map.cellAt(position.x,position.y)==CellType::Floor)
        {
            if ((position == hero_postion)){
                Action ret(ActionType::Attack, dirtoact[ori]);
                return ret;
            }
            else if (!cell_seen(seen,position, startposition, sight))
            {
                Action ret(ActionType::Move, dirtoact[ori]);
                next_cells.push(std::make_tuple(position,1,ret));
            }
        }
    }

    while(!next_cells.empty())
    {   
        int depth; Action ret;
        std::tie(curentposition,depth, ret) = next_cells.front();
        next_cells.pop();
        
        for(auto ori : dir)
        {  
            sf::Vector2i position = curentposition + ori;
            if (position == hero_postion) return ret;
            if ((map.cellAt(position.x,position.y) == CellType::Floor)
                && (depth < sight)
                && (!cell_seen(seen,position, startposition, sight)))
            {
                next_cells.push(std::make_tuple(position,depth+1,ret));
            }
        }

    }
    return Action();
}

Action get_input_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    assert(has_hero(entities));
    return bfs_monster(entity,entities,map);
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
