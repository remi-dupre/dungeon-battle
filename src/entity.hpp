/**
 * \file entity.hpp
 * \brief Representation of entities.
 */

#pragma once

#include <SFML/System/Vector2.hpp>
#include <algorithm>

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
 * \brief Static information about the interaction with the entity
 */
enum class Interaction
{
    None, ///< None
    GoUp, ///< Go up
    GoDown ///< Go down
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
    Entity(EntityType _type, Interaction _interaction, sf::Vector2u _position, Direction _orientation);

    virtual ~Entity() = default;

    /**
     * \brief Return the type of the entity
     */
    EntityType getType() const;

    /**
     * \brief Return the interaction with the entity
     */
    Interaction getInteraction() const;

    /**
     * \brief Return the position of the entity
     */
    sf::Vector2u getPosition() const;

    /**
     * \brief Set the position of the entity
     * \param position The position to set
     */
    void setPosition(sf::Vector2u _position);

    /**
     * \brief Return the orientation of the entity
     */
    Direction getOrientation() const;

    /**
     * \brief Set the orientation of the entity
     * \param position The orientation to set
     */
    void setOrientation(Direction _orientation);

protected:

    const EntityType type; ///< The type of the entity
    const Interaction interaction; ///< The result of the interaction with the entity
    sf::Vector2u position; ///< The position of the entity
    Direction orientation; ///< The orientation of the entity
};


/**
 * \brief Represents a living entity
 */
class Character : public Entity
{
public:

    /**
     * \brief Create a character
     * \param type The type of the character
     * \param interaction The interaction with the entity
     * \param position The position of the character
     * \param orientation The orientation of the character
     * \param hp The hp of the character
     * \param force The force of the character
     */
    Character(EntityType type_,
              Interaction interaction_,
              sf::Vector2u position_,
              Direction orientation_,
              unsigned int hpMax,
              unsigned int strength);

    /**
     * \brief Return the max hp of the character
     */
    unsigned int getHpMax() const;

    /**
     * \brief Set the max hp of the character
     * \param hpMax The hp to set as max
     */
    void setHpMax(unsigned int hpMax);

    /**
     * \brief Return the hp of the character
     */
    unsigned int getHp() const;

    /**
     * \brief Set the hp of the character
     * \param hp The hp to set
     */
    void setHp(unsigned int hp);

    /**
     * \brief Add hp to the character
     * \param hp The hp to add
     */
    void addHp(int hp);

    /**
     * \brief Return the force of the character
     */
    unsigned int getStrength() const;

    /**
     * \brief Set the force of the character
     * \param force The force to set
     */
    void setStrength(unsigned int strength);

protected:

    unsigned int hpMax; ///< The max hp of the character
    unsigned int hp; ///< The hp of the character
    unsigned int strength; ///< The force of the character
};
