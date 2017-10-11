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


Action bfs_monster(const Character& monster, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    // Get information on our monster.
    sf::Vector2i startposition = monster.getPosition();
    int sight = 5; //monster.getSightRadius();


    // Get the position of the hero
    sf::Vector2i heropostion;
    if (has_hero(entities)) heropostion = get_hero_position(entities);
    else return Action(); // add random moves


    //Initialize the minimization of the nom1 distance to the hero
    int save_min_dist = math::distance_1(startposition,heropostion);
    Action save_action = Action();
    if (save_min_dist >= sight) // The monster is to far from the hero.
        return Action(); //comment if you want infinite radius


    // Initiation of the bolean matrix that save which cells are available and not already seen.
    int sightperimeter = 2*sight+1;
    std::vector<std::vector<bool>> seen(sightperimeter, std::vector<bool>(sightperimeter,false) );
    thereisaobstacle(entities,seen,startposition,sight);


    //Initiate of the structures used in the graph BFS.
    sf::Vector2i curentposition = startposition;
    std::queue<std::tuple<sf::Vector2i,int,Action>> next_cells;

    //Initiate the structure saving the directions.
    std::vector<sf::Vector2i> dir ={{1,0},{-1,0},{0,1},{0,-1}};
    std::map<sf::Vector2i,Direction> dirtoact = {
        {{-1,0} , Direction::Left},
        {{1,0}, Direction::Right},
        {{0,-1} , Direction::Up},
        {{0,1}, Direction::Down}};



    //Adding the first cells in the queue.
    for(auto ori : dir)
    {
        sf::Vector2i position = curentposition + ori;
        if (map.cellAt(position.x,position.y)==CellType::Floor)
        {
            if ((position == heropostion)){
                Action ret(ActionType::Attack, dirtoact[ori]);
                return ret;  // The monster is at direct contact with the hero.
            }
            else if (!cell_seen(seen,position, startposition, sight))
            {
                Action ret(ActionType::Move, dirtoact[ori]);
                next_cells.push(std::make_tuple(position,1,ret));
                // The monster needs to move.
            }
        }
    }

    // Dealing with all the cells in the queue.
    while(!next_cells.empty())
    {
        int depth; Action ret;
        std::tie(curentposition,depth, ret) = next_cells.front();
        next_cells.pop(); // Take a cell on the pile


        if (math::distance_1(curentposition,heropostion) < save_min_dist)
            // Is it closer to the hero ?
        {
            save_min_dist = math::distance_1(curentposition,heropostion);
            save_action = ret;  // Then save it.
        }
        
        // See it's closer cells
        for(auto ori : dir)
        {
            sf::Vector2i position = curentposition + ori;
            if (position == heropostion) // Have we fin the hero ?
                // Then do the first move to go toward him.
                return ret; 
            //else if the path is valid.
            if ((map.cellAt(position.x,position.y) == CellType::Floor) 
                && (depth < sight)
                && (!cell_seen(seen,position, startposition, sight)))
            {
                //put it in the queue.
                next_cells.push(std::make_tuple(position,depth+1,ret)); 
            }
        }

    }
    //If we can't go straight to the hero, at least go toward him.
    return save_action;
}

Action get_input_monster(const Character& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    assert(has_hero(entities));
    return bfs_monster(entity,entities,map);
}
