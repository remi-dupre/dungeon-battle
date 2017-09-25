#pragma once

#include <SFML/System/Vector2.hpp>


/**
 * \brief An element that can be put on the floor.
 *
 * An entity should contain any information about its state.
 * Behaviour of an entity should rather be described outside to prevent cyclic dependancies.
 */
class Entity
{
public:
    /**
     * \brief Get position of the entity.
     */
    sf::Vector2i getPosition() const;

protected:
    /**
     * \brief Just saves the map and position.
     */
    Entity(sf::Vector2i position);

    sf::Vector2i position; ///< position of the cell the entity is on
};


/**
 * \brief Defines wether a portal corresponds to an entrance or an exit.
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
     * \brief Creates a portal.
     */
    Portal(sf::Vector2i position, PortalDirection direction);

private:
    PortalDirection direction; ///< Specifie were the portal ends.
};
