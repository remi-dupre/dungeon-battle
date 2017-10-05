#include "ai.hpp"

bool has_hero(const std::vector<std::shared_ptr<Entity>>& entities){
	for(int n = 0; n < entities.size(); n++)
	{
		auto entity = entities[n];
		if(entity->getType() == EntityType::Hero)
		{
			return true;
		}
	}
	return false;
}


sf::Vector2u get_hero_position(const std::vector<std::shared_ptr<Entity>>& entities){
	for(int n = 0; n < entities.size(); n++)
	{
		auto entity = entities[n];
		if(entity->getType() == EntityType::Hero)
		{
			return entity->getPosition();
		}
	}
}

Action get_input_monster(const Entity& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
{
    sf::Vector2u position = entity.getPosition();
    sf::Vector2u hero_postion;
    if (has_hero(entities)) hero_postion = get_hero_position(entities);
    else return just_moving(entity,entities,map);
    return Action();
}

Action just_moving(const Entity& entity, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map)
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