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
    assert(x > 0 && x < width);
    assert(y > 0 && y < height);
    return cells[x + width * y];
}

const CellType& Map::cellAt(int x, int y) const
{
    assert(x > 0 && x < width);
    assert(y > 0 && y < height);
    return cells[x + width * y];
}
