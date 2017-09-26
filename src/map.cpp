#include <cassert>

#include "map.hpp"


Map::Map(unsigned int _width, unsigned int _height) :
    // Copy redundant information about the shape
    width(_width), height(_height),
    // Fill the map with floors
    cells(width * height, CellType::Floor)
{
    assert (width > 0);
    assert (height > 0);
}

#include <iostream>

bool Map::loadFromFile(const std::string& filename)
{
    std::ifstream file;
    file.open(filename);

    if (file.fail())
        return false;

    unsigned int width_, height_;
    file >> width_ >> height_;

    if (file.fail() || height_ == 0 || width_ == 0)
    {
        std::cerr << "Bad map dimensions\n";
        return false;
    }

    // Ignore end of first line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string map_str, line;
    for (unsigned int line_count = 0; line_count < height_; line_count++)
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

CellType& Map::cellAt(unsigned int x, unsigned int y)
{
    assert(x < width);
    assert(y < height);
    return cells[x + width * y];
}

const CellType& Map::cellAt(unsigned int x, unsigned int y) const
{
    assert(x < width);
    assert(y < height);
    return cells[x + width * y];
}
