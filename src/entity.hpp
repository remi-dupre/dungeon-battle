#pragma once

#include <SFML/System/Vector2.hpp>

enum class EntityType
{
    Hero,
    Monster,
    Stairs
}

enum class Direction
{
    Up,
    Down,
    Left,
    Right
}

class Entity
{
public:

    Entity(sf::Vector2i position, int orientation, EntityType type);
    sf::Vector2i getPosition() const;
    void setPosition(sf::Vector2i position);

    int getOrientation() const;
    void setOrientation(int orientation);
    
protected:

    sf::Vector2i position;
    Direction orientation;
    EntityType type;

};
