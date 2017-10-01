inline Action get_input_monster(const Entity& entity, const std::vector<std::shared_ptr<Entity>>& entities, const std::function<const CellType&(unsigned int, unsigned int)>& cell_at)
{
    sf::Vector2u position = entity.getPosition();

    if (cell_at(position.x-1, position.y) == CellType::Floor)
        return Action(ActionType::Move, Direction::Left);
    if (cell_at(position.x+1, position.y) == CellType::Floor)
        return Action(ActionType::Move, Direction::Right);
    if (cell_at(position.x, position.y-1) == CellType::Floor)
        return Action(ActionType::Move, Direction::Up);
    if (cell_at(position.x, position.y+1) == CellType::Floor)
        return Action(ActionType::Move, Direction::Down);
    
    return Action();
}
