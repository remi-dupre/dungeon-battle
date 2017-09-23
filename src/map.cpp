#include "map.hpp"


Map::Map(int _width, int _height) :
    // Copy redundant information about the shape
    width(_width), height(_height),
    // Fill the map with floors
    cells(width * height, CellType::Floor)
{}


CellType& Map::tileAt(int x, int y)
{
    return cells[x + width * y];
}
