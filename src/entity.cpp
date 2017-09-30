#include "entity.hpp"
#include "utility.hpp"


Entity::Entity(EntityType _type, sf::Vector2u _position, Direction _orientation) :
    type(_type),
    position(_position),
    orientation(_orientation)
{}

EntityType Entity::getType() const
{
    return type;
}

sf::Vector2u Entity::getPosition() const
{
    return position;
}

void Entity::setPosition(sf::Vector2u _position)
{
    position = _position;
}

Direction Entity::getOrientation() const
{
    return orientation;
}

void Entity::setOrientation(Direction _orientation)
{
    if (_orientation != Direction::None)
        orientation = _orientation;
}
