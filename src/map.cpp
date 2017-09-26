#include <cassert>

#include "map.hpp"


Map::Map(int _width, int _height) :
    // Copy redundant information about the shape
    width(_width), height(_height),
    // Fill the map with floors
    cells(width * height, CellType::Floor)
{}

#include <iostream>

bool Map::loadFromFile(std::string filename)
{
    std::ifstream file;
    file.open(filename);

    if (file.fail())
        return false;

    int width_, height_;
    file >> width_ >> height_;

    if (file.fail() || height_ <= 0 || width_ <= 0)
    {
        std::cerr << "Bad map dimensions\n";
        return false;
    }

    // Ignore end of first line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string map_str, line;
    for (int line_count = 0; line_count < height_; line_count++)
    {
        std::getline(file, line);
        map_str.append(line);
    }

    if (file.fail())
    {
        std::cerr << "Bad map data\n";
        return false;
    }

    cells.clear();

    // Create the map from the string representing it
    std::transform(
        std::begin(map_str), std::end(map_str),
        std::back_inserter(cells),
        [] (const char& c) -> CellType
        {
            switch (c)
            {
                case '#':
                    return CellType::Wall;
                    break;
                case '.':
                    return CellType::Floor;
                    break;
                default:
                    return CellType::Empty;
                    break;
            }
        }
    );

    width = width_;
    height = height_;

    return true;
}

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
