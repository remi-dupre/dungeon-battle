 /**
 * \file control.hpp
 * \brief Retrieve actions performed by entities.
 */

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "config.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "ai.hpp"


/**
 * \brief A type of action
 */
enum class ActionType
{
    None, ///< No action
    Move, ///< Move action
    Attack, ///< Attack action
    Interact, ///< Interact action
    Pause ///< Pause and menu
};

/**
 * \brief Represents an action
 */
struct Action
{
    ActionType type; ///< Type of the action
    Direction direction; ///< Direction in which the action is performed

    /**
     * \brief Default constructor
     *
     * This creates an action with no type and no direction.
     */
    explicit Action();

    /**
     * \brief Create an action with no direction
     * \param type The type of the action
     */
    explicit Action(ActionType type);

    /**
     * \brief Create an action
     * \param type The type of the action
     * \param direction The direction of the action
     */
    explicit Action(ActionType type, Direction direction);
};

namespace control
{
    /**
     * \brief Return an action performed by an entity
     */
    Action get_input(const Entity& entity,
                     const std::vector<std::shared_ptr<Entity>>& entities,
                     const Map& map,
                     const Configuration& config);
}
