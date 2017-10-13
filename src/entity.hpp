/**
 * \file entity.hpp
 * \brief Representation of entities.
 */

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

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
    Stairs, ///< A staircase
    Item
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
    Entity(EntityType type, Interaction interaction, sf::Vector2i position, Direction orientation);

    virtual ~Entity() = default;

    /**
     * \brief Return the id of the entity
     */
    unsigned int getId() const;

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
     * \param orientation The orientation to set
     */
    void setOrientation(Direction orientation);

    /**
     * \brief Return the position at the previous move of the entity
     */
    sf::Vector2i getOldPosition() const;

    /**
     * \brief Return true if the entity is currently moving
     */
    bool isMoving() const;

    /**
     * \brief Set if the entity is currently moving
     */
    void setMoving(bool moving);

    /**
     * \brief Return true if the entity is currently attacking
     */
    bool isAttacking() const;

    /**
     * \brief Set if the entity is currently attacking
     */
    void setAttacking(bool attacking);

    /**
     * \brief Return true if the entity is currently attacked
     */
    bool isAttacked() const;

    /**
     * \brief Set if the entity is currently attacked
     */
    void setAttacked(bool attacked);


    /**
     * \brief Return the zIndex of the entity
     */
    int zIndex();

protected:

    const unsigned int id; ///< The id of the entity
    const EntityType type; ///< The type of the entity
    const Interaction interaction; ///< The result of the interaction with the entity
    sf::Vector2i position; ///< The position of the entity
    Direction orientation; ///< The orientation of the entity

    sf::Vector2i old_position; ///< The position of the entity at the previous turn
    bool moving; ///< Tells whether the entity is currently moving or not
    bool attacking; ///< Tells whether the entity is currently attacking or not
    bool attacked; ///< Tells whether the entity is currently attacked or not
};


class Item : public Entity
{
public:
    Item(const std::string& name, const sf::Vector2i& position);

protected:
    std::string name;
    int level;
    int experience;
    int hpMax;
    int hp;
    int strength;
    int defense;
    int sightRadius;
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
     * \param hpMax The maximum hp of the character
     * \param strength The force of the character
     */
    Character(EntityType type,
              Interaction interaction,
              sf::Vector2i position,
              Direction orientation,
              unsigned int hpMax,
              unsigned int strength);

    /**
     * \brief Return the max hp of the character
     */
    unsigned int getLevel() const;

    /**
     * \brief Set the max hp of the character
     * \param hpMax The hp to set as max
     */
    void setLevel(unsigned int level);

    /**
     * \brief Increment the level of the character
     */
    void levelUp();

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
     * \brief Return the living status of the character
     */
    bool isAlive();

    /**
     * \brief Return the strength of the character
     */
    unsigned int getStrength() const;

    /**
     * \brief Set the strength of the character
     * \param strength The strength to set
     */
    void setStrength(unsigned int strength);

    /**
     * \brief Return the defense of the character
     */
    unsigned int getDefense() const;
    
    /**
     * \brief Set the defense of the character
     * \param defense The defense to set
     */
    void setDefense(unsigned int defense);

    /**
     * \brief Return the sight radius of the character
     */
    unsigned int getSightRadius() const;

    /**
     * \brief Set the sight radius of the character
     * \param sightRadius The sight radius to set
     */
    void setSightRadius(unsigned int sightRadius);


    /**
     * \brief Return the experience of the character
     */
    unsigned int getExperience() const;

    /**
     * \brief Set the experience of the character
     * \param experience The experience to set
     */
    void setExperience(unsigned int experience);

    /**
     * \brief Add experience to the character
     * \param experience The experience to add
     */
    void addExperience(unsigned int experience);

protected:

    unsigned int level; ///< The level of the character
    std::function<unsigned int(unsigned int)> experienceCurve; ///< The experience curve of the character
    unsigned int experience; ///< The experience of the character
    unsigned int hpMax; ///< The max hp of the character
    unsigned int hp; ///< The hp of the character
    unsigned int strength; ///< The force of the character
    unsigned int defense; ///< The defense of the character
    unsigned int sightRadius; ///< The sight radius of the character (0 for infinity)
};

/**
 * \brief Test if a vector of entities contains a hero
 * \param entities A vector of entities
 */
bool has_hero(const std::vector<std::shared_ptr<Entity>>& entities);

/**
 * \brief Return the position of the first hero in a vector of entities
 * \param entities A vector of entities containing a hero
 */
sf::Vector2i get_hero_position(const std::vector<std::shared_ptr<Entity>>& entities);
