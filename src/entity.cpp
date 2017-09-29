#include "entity.hpp"

Entity::Entity(sf::Vector2i _position,
               Direction _orientation,
               EntityType _type) :
    position(_position),
    orientation(_orientation),
    type(_type)
{}


sf::Vector2i Entity::getPosition() const
{
    return position;
}

void Entity::setPosition(sf::Vector2i _position)
{
    position = _position;
}

Direction Entity::getOrientation() const
{
    return orientation;
}

void Entity::setOrientation(Direction _orientation)
{
    orientation = _orientation;
}
