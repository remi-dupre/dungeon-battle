/**
 * \file entity.hpp
 * \brief Representation of entities.
 */

#pragma once

#include <SFML/System/Vector2.hpp>

#include "utility.hpp"


/**
 * \brief Static information about the type of the entity
 */
enum class EntityType
{
    None, ///< Missing type
    Hero, ///< A hero
    Monster, ///< A monster
    Stairs ///< A staircase
};

/**
 * \brief Represents anything that is interactive
 */
class Entity
{
public:

    /**
     * \brief Create an entity
     * \param type The type of the entity
     * \param position The position of the entity
     * \param orientation The orientation of the entity
     */
    Entity(EntityType type, sf::Vector2u position, Direction orientation);

    /**
     * \brief Return the type of the entity
     */
    EntityType getType() const;

    /**
     * \brief Return the position of the entity
     */
    sf::Vector2u getPosition() const;

    /**
     * \brief Set the position of the entity
     * \param position The position to set
     */
    void setPosition(sf::Vector2u position);

    /**
     * \brief Return the orientation of the entity
     */
    Direction getOrientation() const;

    /**
     * \brief Set the orientation of the entity
     * \param position The orientation to set
     */
    void setOrientation(Direction orientation);

protected:

    EntityType type; ///< The type of the entity
    sf::Vector2u position; ///< The position of the entity
    Direction orientation; ///< The orientation of the entity
};
