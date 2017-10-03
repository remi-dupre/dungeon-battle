#include "entity.hpp"
#include "utility.hpp"


Entity::Entity(EntityType type_, sf::Vector2u position_, Direction orientation_) :
    type(type_),
    position(position_),
    orientation(orientation_)
{}

EntityType Entity::getType() const
{
    return type;
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


Character::Character(EntityType type_, sf::Vector2u position_, Direction orientation_, unsigned int hp_, unsigned int force_) :
    Entity(type_, position_, orientation_),
    hp(hp_),
    force(force_)
{}

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
    (static_cast<int>(hp) < -hp_) ? hp = 0 : hp += hp_; // Berk
}

unsigned int Character::getForce() const
{
    return force;
}

void Character::setForce(unsigned int force_)
{
    force = force_;
}
