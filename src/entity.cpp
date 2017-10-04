#include <algorithm>

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
    hpMax(hpMax_),
    hp(hpMax_),
    strength(strength_),
    sightRadius(0)
{}

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
    (static_cast<int>(hp) < -hp_) ? hp = 0 : hp = std::min(hp+hp_, hpMax);
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
