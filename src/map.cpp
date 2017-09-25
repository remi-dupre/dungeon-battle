#include <cassert>

#include "map.hpp"


Map::Map(int _width, int _height) :
    // Copy redundant information about the shape
    width(_width), height(_height),
    // Fill the map with floors
    cells(width * height, CellType::Floor)
{}

int Map::getWidth() const
{
    return width;
}

int Map::getHeight() const
{
    return height;
}

CellType& Map::cellAt(int x, int y)
{
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    return cells[x + width * y];
}

const CellType& Map::cellAt(int x, int y) const
{
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    return cells[x + width * y];
}

const std::vector<Entity>& Map::getEntities() const
{
    return entities;
}

std::vector<Entity> Map::getEntitiesOnCell(int x, int y) const
{
    return getEntitiesOnCell({x, y});
}

std::vector<Entity> Map::getEntitiesOnCell(sf::Vector2i position) const
{
    std::vector<Entity> entities_on_cell;
    // Runs a copy on 'entities' outputed to 'entities_on_cell'
    std::copy_if(
        std::begin(entities), std::end(entities),
        std::back_inserter(entities_on_cell),
        [position](const Entity& e) -> bool {
            return e.getPosition() == position;
        }
    );
    return entities_on_cell;
}
