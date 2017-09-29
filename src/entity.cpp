#include "entity.hpp"


Entity::Entity(sf::Vector2i _position, Direction _orientation) :
    position(_position),
    orientation(_orientation)
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
