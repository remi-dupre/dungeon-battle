/**
 * \file entity.hpp
 * \brief Representation of entities.
 */

#pragma once

#include <SFML/System/Vector2.hpp>


/**
 * \brief Static information about the type of the entity
 */
enum class EntityType
{
    Hero, ///< A hero
    Monster, ///< A monster
    Stairs ///< A staircase
};

/**
 * \brief Dynamic information about the orientation of the entity
 */
enum class Direction
{
    Left, ///< Left
    Right ///< Right
    Up, ///< Up
    Down, ///< Down
};


/**
 * \brief Represents anything that is interactive
 */
class Entity
{
public:

    /**
     * \brief Create an entity
     * \param position The position of the entity
     * \param orientation The orientation of the entity
     */
    Entity(sf::Vector2i position, Direction orientation);

    /**
     * \brief Return the position of the entity
     */
    sf::Vector2i getPosition() const;

    /**
     * \brief Set the position of the entity
     * \param position The position to set
     */
    void setPosition(sf::Vector2i position);

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

    sf::Vector2i position; ///< The position of the entity
    Direction orientation; ///< The orientation of the entity
    EntityType type; ///< The type of the entity
};
