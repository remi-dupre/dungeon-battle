/**
 * \file entity.hpp
 * \brief Representation of entities.
 */

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
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
    Item ///< An item
};


/**
 * \brief Static information about the interaction with the entity
 */
enum class Interaction
{
    None, ///< None
    GoUp, ///< Go up
    GoDown, ///< Go down
    PickUp, ///< Pick up
    Use ///< Use
};


/**
 * \brief Represents anything that is interactive
 */
class Entity : public std::enable_shared_from_this<Entity>
{
public:

    /**
     * \brief Create an entity
     * \param type The type of the entity
     * \param position The position of the entity
     * \param orientation The orientation of the entity
     */
    Entity(EntityType type, Interaction interaction, sf::Vector2i position, Direction orientation);

    /**
     * \brief Create an entity
     * \param type The type of the entity
     * \param position The position of the entity
     * \param orientation The orientation of the entity
     * \param controller_id The id of the controller
     */
    Entity(EntityType type, Interaction interaction, sf::Vector2i position, Direction orientation, unsigned int controller_id);

    virtual ~Entity() = default;

    /**
     * \brief Return the id of the entity
     */
    unsigned int getId() const;

    /**
     * \brief Return the id of the controller of the entity
     */
    unsigned int getControllerId() const;

    /**
     * \brief Return the type of the entity
     */
    EntityType getType() const;

    /**
     * \brief Return a pointer to a copy of this entity.
     * \todo  A real implementation from someone who know cpp -- Rémi
     */
    std::shared_ptr<Entity> copy() const;

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
    const unsigned int controller_id; ///< The id of the controller of the entity
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

    /**
     * \brief Create an item
     * \param name The name of the item
     * \param position The position of the item
     * \param level The level modifier of the item
     * \param experience The experience modifier of the item
     * \param hpMax The hp max modifier of the item
     * \param hp The hp modifier of the item
     * \param strength The strength modifier of the item
     * \param defense The defense modifier of the item
     * \param sighRadius The sight radius modifier of the item
     */
    Item(const std::string& name,
        const sf::Vector2i& position,
        int level,
        int experience,
        int hpMax,
        int hp,
        int strength,
        int defense,
        int sightRadius);

    /**
     * \brief Return the level modifier of the item
     */
    int getLevel() const;

    /**
     * \brief Return the experience modifier of the item
     */
    int getExperience() const;

    /**
     * \brief Return the hp max modifier of the item
     */
    int getHpMax() const;

    /**
     * \brief Return the hp modifier of the item
     */
    int getHp() const;

    /**
     * \brief Return the strength modifier of the item
     */
    int getStrength() const;

    /**
     * \brief Return the defense modifier of the item
     */
    int getDefense() const;

    /**
     * \brief Return the sight radius modifier of the item
     */
    int getSightRadius() const;

protected:

    std::string name; ///< The name of the item
    int level; ///< The level modifier of the item
    int experience; ///< The experience modifier of the item
    int hpMax; ///< The max hp modifier of the item
    int hp; ///< The hp modifier of the item
    int strength; ///< The strength modifier of the item
    int defense; ///< The defense modifier of the item
    int sightRadius; ///< sightRadius modifier of the item
};

/**
 * \brief Represents a living entity
 */
class Character : public Entity
{
public:

    /**
     * \brief Create a character owned by the AI
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
     * \brief Create a character
     * \param type The type of the character
     * \param interaction The interaction with the entity
     * \param position The position of the character
     * \param orientation The orientation of the character
     * \param hpMax The maximum hp of the character
     * \param strength The force of the character
     * \param controller_id The owner of the character
     */
    Character(EntityType type,
              Interaction interaction,
              sf::Vector2i position,
              Direction orientation,
              unsigned int hpMax,
              unsigned int strength,
              unsigned int controller_id);

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
    bool isAlive() const;

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

    /**
     * \brief Award experience to the character for a killing blow
     * \param target The slained character
     */
    void awardExperience(const Character& target);

    /**
     * \brief Check if there is an empty inventory slot
     */
    bool roomInInventory();

    /**
     * \brief Add an item to the inventory, asserting roomInInventory()
     * \param item The item to add
     */
    void pickUp(Item item);

protected:

    unsigned int level; ///< The level of the character
    std::function<unsigned int(unsigned int)> experienceCurve; ///< The experience curve of the character
    unsigned int experience; ///< The experience of the character
    unsigned int hpMax; ///< The max hp of the character
    unsigned int hp; ///< The hp of the character
    unsigned int strength; ///< The force of the character
    unsigned int defense; ///< The defense of the character
    unsigned int sightRadius; ///< The sight radius of the character (0 for infinity)

    std::vector<Item> inventory; ///< The inventory of the character
    unsigned int inventorySize; ///< The size of the inventory of the character
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
