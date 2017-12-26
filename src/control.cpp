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

Action::Action(ActionType type_) :
    type(type_),
    direction(Direction::None)
{}

Action::Action(ActionType type_, Direction direction_) :
    type(type_),
    direction(direction_)
{}

inline Action get_input_hero(const Configuration& config)
{
    if (sf::Keyboard::isKeyPressed(config.left_key))
        return Action(ActionType::Move, Direction::Left);
    if (sf::Keyboard::isKeyPressed(config.right_key))
        return Action(ActionType::Move, Direction::Right);
    if (sf::Keyboard::isKeyPressed(config.up_key))
        return Action(ActionType::Move, Direction::Up);
    if (sf::Keyboard::isKeyPressed(config.down_key))
        return Action(ActionType::Move, Direction::Down);
    if (sf::Keyboard::isKeyPressed(config.interaction_key))
        return Action(ActionType::Interact, Direction::None);
    if (sf::Keyboard::isKeyPressed(config.attack_left_key))
        return Action(ActionType::Attack, Direction::Left);
    if (sf::Keyboard::isKeyPressed(config.attack_right_key))
        return Action(ActionType::Attack, Direction::Right);
    if (sf::Keyboard::isKeyPressed(config.attack_up_key))
        return Action(ActionType::Attack, Direction::Up);
    if (sf::Keyboard::isKeyPressed(config.attack_down_key))
        return Action(ActionType::Attack, Direction::Down);
    if (sf::Keyboard::isKeyPressed(config.menu_key))
        return Action(ActionType::Pause);
    return Action();
}

Action control::get_input(const Entity& entity,
                          const std::vector<std::shared_ptr<Entity>>& entities,
                          const Map &map,
                          const Configuration& config)
{
    switch (entity.getType())
    {
        case EntityType::Hero:
        case EntityType::Monster:
            if (entity.getControllerId())
                return get_input_hero(config);
            else
                return get_input_monster(static_cast<const Character&>(entity), entities, map);
            break;
        default:
            return Action();
            break;
    }
}
