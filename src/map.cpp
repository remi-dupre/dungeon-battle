#include <cassert>

#include "map.hpp"


Map::Map() :
    Map(1, 1)
{}

Map::Map(int width_, int height_) :
    // Copy redundant information about the shape
    width(width_), height(height_),
    // Fill the map with floors
    cells(width * height, CellType::Empty)
{
    assert (width > 0);
    assert (height > 0);
}

bool Map::loadFromFile(const std::string& filename)
{
    std::ifstream file;
    file.open(filename);

    if (file.fail())
        return false;

    file >> *this;

    return true;
}

void Map::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);

    if (!file.is_open())
        return;

    file << *this;

    return;
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

std::ostream& operator<<(std::ostream& stream, const Map& map)
{
    stream << map.width << ' ' << map.height << std::endl;

    for (int y = 0; y < map.height; y++)
    {
        for (int x = 0; x < map.width; x++)
        {
            switch (map.cellAt(x, y))
            {
                case CellType::Wall:
                    stream << '#';
                    break;
                case CellType::Floor:
                    stream << '.';
                    break;
                default:
                    stream << ' ';
                    break;
            }
        }

        stream << std::endl;
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, Map& map)
{
    int width_, height_;
    stream >> width_ >> height_;

    if (stream.fail() || height_ == 0 || width_ == 0)
    {
        std::cerr << "Bad map dimensions\n";
        return stream;
    }

    // Ignore end of first line
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string map_str, line;
    for (int line_count = 0; line_count < height_; line_count++)
    {
        std::getline(stream, line);

        if (line.size() != width_)
        {
            std::cerr << "Bad map data\n";
            return stream;
        }

        map_str.append(line);
    }

    if (stream.fail())
    {
        std::cerr << "Bad map data\n";
        return stream;
    }

    map.cells.clear();

    // Loads the map from the string representing it
    std::transform(
        std::begin(map_str), std::end(map_str),
        std::back_inserter(map.cells),
        [](const char& c) -> CellType
        {
            switch (c)
            {
                case '#':
                    //return CellType::Wall;
                    break;
                case '.':
                    //return CellType::Floor;
                    break;
                default:
                    //return CellType::Empty;
                    break;
            }
        }
    );

    map.width = width_;
    map.height = height_;

    return stream;
}

Map& Map::operator=(Map&& other)
{
    width = other.width;
    height = other.height;
    cells = std::move(other.cells);
    return *this;
}
