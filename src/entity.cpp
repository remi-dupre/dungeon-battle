#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <vector>

#include "entity.hpp"
#include "utility.hpp"


Entity::Entity(EntityType type_, Interaction interaction_, sf::Vector2u position_, Direction orientation_) :
    type(type_),
    interaction(interaction_),
    position(position_),
    orientation(orientation_)
{}

EntityType Entity::getType() const
{
    return type;
}

Interaction Entity::getInteraction() const
{
    return interaction;
}

sf::Vector2u Entity::getPosition() const
{
    return position;
}

void Entity::setPosition(sf::Vector2u position_)
{
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


Character::Character(EntityType type_,
                     Interaction interaction_,
                     sf::Vector2u position_,
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
    sightRadius(0)
{}


unsigned int Character::getLevel() const
{
    return level;
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
    return hpMax;
}

void Character::setHpMax(unsigned int hpMax_)
{
    hpMax = hpMax_;
}

unsigned int Character::getHp() const
{
    return hp;
}

void Character::setHp(unsigned int hp_)
{
    hp = hp_;
}

void Character::addHp(int hp_)
{
    (static_cast<int>(hp) < hp_) ? hp = 0 : hp = std::min(hp+hp_, hpMax);
}

unsigned int Character::getStrength() const
{
    return strength;
}

void Character::setStrength(unsigned int strength_)
{
    strength = strength_;
}

unsigned int Character::getSightRadius() const
{
    return sightRadius;
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


sf::Vector2u get_hero_position(const std::vector<std::shared_ptr<Entity>>& entities){
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