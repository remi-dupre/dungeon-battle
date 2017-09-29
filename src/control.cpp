#include <functional>
#include <memory>
#include <vector>

#include <SFML/Window.hpp>

#include "control.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "utility.hpp"


Action::Action() :
    type(ActionType::None),
    direction(Direction::None)
{}

Action::Action(ActionType _type) :
    type(_type),
    direction(Direction::None)
{}

Action::Action(ActionType _type, Direction _direction) :
    type(_type),
    direction(_direction)
{}

Action get_input_hero(const Configuration& config)
{
    if (sf::Keyboard::isKeyPressed(config.left_key))
        return Action(ActionType::Move, Direction::Left);
    if (sf::Keyboard::isKeyPressed(config.right_key))
        return Action(ActionType::Move, Direction::Right);
    if (sf::Keyboard::isKeyPressed(config.up_key))
        return Action(ActionType::Move, Direction::Up);
    if (sf::Keyboard::isKeyPressed(config.down_key))
        return Action(ActionType::Move, Direction::Down);
    
    return Action();
}

Action control::get_input(const Entity& entity, const std::vector<std::shared_ptr<Entity>>& entities, const std::function<const CellType&(unsigned int, unsigned int)>& cell_at, const Configuration& config)
{
    switch (entity.getType())
    {
        case EntityType::Hero:
            return get_input_hero(config);
            break;
        default:
            return Action();
            break;
    }
}
