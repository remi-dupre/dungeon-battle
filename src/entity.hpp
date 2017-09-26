/**
 * \file entity.hpp
 * \brief Representation of entities.
 */

#pragma once

#include <SFML/System/Vector2.hpp>


/**
 * \brief An element that can be put on the floor
 *
 * An entity should contain any information about its state.
 * Behavior of an entity should rather be described outside to prevent cyclic dependancies.
 */
class Entity
{
protected:
    /**
     * \brief Just saves the map and position.
     */
    Entity(sf::Vector2i pos);

    sf::Vector2i pos; ///< Position of the cell the entity is on
};


/**
 * \brief Defines whether a portal corresponds to an entrance or an exit.
 */
enum class PortalDirection
{
    NextLvl,
    PrevLvl
};


/**
 * \brief A warp to next level or from previous level
 */
class Portal : public Entity
{
public:
    /**
     * \brief Creates a portal
     */
    Portal(sf::Vector2i pos, PortalDirection direction);

private:
    PortalDirection direction; ///< Specify where the portal ends
};
