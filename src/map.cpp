#include <cassert>
#include <fstream>

#include "map.hpp"


Map::Map() :
    Map(1, 1)
{}

Map::Map(int width_, int height_) :
    width(width_), height(height_),
    cells(width * height, CellType::Empty)
{
    assert (width > 0);
    assert (height > 0);
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
    assert(x < width);
    assert(y < height);
    assert(x >= 0);
    assert(y >= 0);

    return cells[x + width * y];
}

CellType& Map::cellAt(sf::Vector2i coords)
{
    return cellAt(coords.x, coords.y);
}

CellType Map::cellAt(int x, int y) const
{
    if (x < 0 || y < 0 || x > width || y > height)
        return CellType::Empty;
    return cells[x + width * y];
}

CellType Map::cellAt(sf::Vector2i coords) const
{
    return cellAt(coords.x, coords.y);
}

bool Map::wallNext(int x, int y) const
{
    return wallNext({x, y});
}

bool Map::wallNext(sf::Vector2i coords) const
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (cellAt(coords.x + i, coords.y + j) == CellType::Wall)
                return true;
        }
    }

    return false;
}

Map& Map::operator=(Map&& other)
{
    width = other.width;
    height = other.height;
    cells = std::move(other.cells);
    return *this;
}


bool Map::loadFromFile(const std::string& filename)
{
    std::ifstream file;
    file.open(filename, std::ios::binary);

    if (file.fail())
        return false;

    file >> *this;

    return true;
}

void Map::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename, std::ios::trunc | std::ios::binary);

    if (!file.is_open())
        return;

    file << *this;

    return;
}

std::ostream& operator<<(std::ostream& stream, const Map& map)
{
    stream.write(reinterpret_cast<const char*>(&map.width), sizeof(int32_t));
    stream.write(reinterpret_cast<const char*>(&map.height), sizeof(int32_t));

    stream.write(reinterpret_cast<const char*>(map.cells.data()), map.width * map.height);

    return stream;
}

std::istream& operator>>(std::istream& stream, Map& map)
{
    int32_t width_, height_;
    stream.read(reinterpret_cast<char*>(&width_), sizeof(int32_t));
    stream.read(reinterpret_cast<char*>(&height_), sizeof(int32_t));

    if (stream.fail() || height_ <= 0 || width_ <= 0)
    {
        std::cerr << "Bad map dimensions\n";
        return stream;
    }

    std::vector<CellType> cells(width_ * height_, CellType::Empty);
    stream.read(reinterpret_cast<char*>(cells.data()), width_ * height_);

    if (stream.fail())
    {
        std::cerr << "Bad map data\n";
        return stream;
    }

    map.width = width_;
    map.height = height_;
    std::swap(cells, map.cells);

    return stream;
}
