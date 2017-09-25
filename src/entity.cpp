#include "entity.hpp"

Entity::Entity(sf::Vector2i _pos) :
    pos(_pos)
{}


Portal::Portal(sf::Vector2i _pos, PortalDirection _direction) :
    Entity::Entity(_pos),
    direction(_direction)
{}
