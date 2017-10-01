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
