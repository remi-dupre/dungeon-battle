#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "config.hpp"
#include "entity.hpp"
#include "map.hpp"


enum class ActionType
{
    None,
    Move,
    Attack,
    Interract
};

struct Action
{
    ActionType type; ///< Type of the action
    Direction direction; ///< Direction in which the action is performed

    /**
     * \brief Default constructor
     * 
     * This creates an action with no type and no direction.
     */
    Action();

    /**
     * \brief 
     * 
     *
     */
    Action(ActionType _type);
    Action(ActionType _type, Direction _direction);
};

namespace control
{
    /**
     * \brief Return an action performed by an entity
     */
    Action get_input(const Entity& entity, const std::vector<std::shared_ptr<Entity>>& entities, std::function<const CellType&(unsigned int, unsigned int)> cell_at, const Configuration& config);
};
