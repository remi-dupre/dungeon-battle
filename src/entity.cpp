#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <vector>

#include "entity.hpp"
#include "utility.hpp"

unsigned int currentId = 0;

Entity::Entity(EntityType type_, Interaction interaction_, sf::Vector2i position_, Direction orientation_) :
    id(++currentId),
    type(type_),
    interaction(interaction_),
    position(position_),
    orientation(orientation_),
    moving(false),
    attacking(false),
    attacked(false)
{}


unsigned int Entity::getId() const
{
    return id;
}

EntityType Entity::getType() const
{
    return type;
}

Interaction Entity::getInteraction() const
{
    return interaction;
}

sf::Vector2i Entity::getPosition() const
{
    return position;
}

void Entity::setPosition(sf::Vector2i position_)
{
    old_position = position;
    position = position_;
}

Direction Entity::getOrientation() const
{
    return orientation;
}

void Entity::setOrientation(Direction orientation_)
{
    if (orientation_ != Direction::None)
        orientation = orientation_;
}

sf::Vector2i Entity::getOldPosition() const
{
    return old_position;
}

void Entity::setMoving(bool moving_)
{
    moving = moving_;
}

bool Entity::isMoving() const
{
    return moving;
}

void Entity::setAttacking(bool attacking_)
{
    attacking = attacking_;
}

bool Entity::isAttacking() const
{
    return attacking;
}

void Entity::setAttacked(bool attacked_)
{
    attacked = attacked_;
}

bool Entity::isAttacked() const
{
    return attacked;
}

int Entity::zIndex()
{
    switch(type)
    {
    case EntityType::None:
        return 0;
    case EntityType::Hero:
        return 1;
    case EntityType::Monster:
        return 1;
    case EntityType::Stairs:
        return 0;
    default:
        return 0;
    }
}



Item::Item(const std::string& name_, const sf::Vector2i& position_) :
    Entity(EntityType::Item, Interaction::None, position_, Direction::None),
    name(name_),
    level(0),
    experience(0),
    hpMax(0),
    hp(0),
    strength(0),
    defense(0),
    sightRadius(0)
{}

int Item::getLevel() const
{
    return level;
}

int Item::getExperience() const
{
    return experience;
}

int Item::getHpMax() const
{
    return hpMax;
}

int Item::getHp() const
{
    return hp;
}

int Item::getStrength() const
{
    return strength;
}

int Item::getDefense() const
{
    return defense;
}

int Item::getSightRadius() const
{
    return sightRadius;
}




Character::Character(EntityType type_,
                     Interaction interaction_,
                     sf::Vector2i position_,
                     Direction orientation_,
                     unsigned int hpMax_,
                     unsigned int strength_) :
    Entity(type_, interaction_, position_, orientation_),
    level(1),
    experienceCurve([](unsigned int level) -> unsigned int {return 10*level;}),
    experience(0),
    hpMax(hpMax_),
    hp(hpMax_),
    strength(strength_),
    defense(0),
    sightRadius(0),
    inventory(std::vector<Item>()),
    inventorySize(-1)
{}


unsigned int Character::getLevel() const
{
    unsigned int c = level;
    for (const Item& item : inventory)
    {
        c += item.getLevel();
    }
    return c;
}

void Character::setLevel(unsigned int level_)
{
    level = level_;
}

void Character::levelUp()
{
    if (experience > experienceCurve(level))
    {
        experience -= experienceCurve(level);
        level++;
        levelUp();
    }
}

unsigned int Character::getHpMax() const
{
    unsigned int c = hpMax;
    for (const Item& item : inventory)
    {
        c += item.getHpMax();
    }
    return c;
}

void Character::setHpMax(unsigned int hpMax_)
{
    hpMax = hpMax_;
}

unsigned int Character::getHp() const
{
    unsigned int c = hp;
    for (const Item& item : inventory)
    {
        c += item.getHp();
    }
    return c;
}

void Character::setHp(unsigned int hp_)
{
    hp = hp_;
}

void Character::addHp(int hp_)
{
    (static_cast<int>(hp) < -hp_) ? hp = 0 : hp = std::min(hp+hp_, hpMax);
}

bool Character::isAlive()
{
    return (getHp() > 0);
}

unsigned int Character::getStrength() const
{
    unsigned int c = strength;
    for (const Item& item : inventory)
    {
        c += item.getStrength();
    }
    return c;
}

void Character::setStrength(unsigned int strength_)
{
    strength = strength_;
}

unsigned int Character::getDefense() const
{
    unsigned int c = defense;
    for (const Item& item : inventory)
    {
        c += item.getDefense();
    }
    return c;
}

void Character::setDefense(unsigned defense_)
{
    defense = defense_;
}

unsigned int Character::getSightRadius() const
{
    unsigned int c = sightRadius;
    for (const Item& item : inventory)
    {
        c += item.getSightRadius();
    }
    return c;
}

void Character::setSightRadius(unsigned int sightRadius_)
{
    sightRadius = sightRadius_;
}

unsigned int Character::getExperience() const
{
    return experience;
}

void Character::setExperience(unsigned int experience_)
{
    experience = experience_;
}

void Character::addExperience(unsigned int experience_)
{
    experience += experience_;
    levelUp();
}

bool Character::roomInInventory()
{
    return (inventorySize > inventory.size());
}

void Character::pickUp(Item item)
{
    inventory.push_back(item);
}



bool has_hero(const std::vector<std::shared_ptr<Entity>>& entities){
    for(const auto& entity : entities)
    {
        if(entity->getType() == EntityType::Hero)
        {
            return true;
        }
    }
    return false;
}


sf::Vector2i get_hero_position(const std::vector<std::shared_ptr<Entity>>& entities){
    for(const auto& entity : entities)
    {
        if(entity->getType() == EntityType::Hero)
        {
            return entity->getPosition();
        }
    }
    assert(false);
    return {0, 0};
}
