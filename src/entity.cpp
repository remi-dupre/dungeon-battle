#include "entity.hpp"

Entity::Entity(sf::Vector2i _position) :
    position(_position)
{}

sf::Vector2i Entity::getPosition() const
{
    return position;
}


Portal::Portal(sf::Vector2i _position, PortalDirection _direction) :
    Entity::Entity(_position),
    direction(_direction)
{}
