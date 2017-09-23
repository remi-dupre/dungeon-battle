#include "map.hpp"


Map::Map(int _width, int _height) :
    // Copy redundant information about the shape
    width(_width), height(_height),
    // Fill the map with floors
    cells(_width, std::vector<CellType>(_height, CellType::Floor))
{}
