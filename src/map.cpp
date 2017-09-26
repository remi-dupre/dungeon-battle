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

std::ostream &operator<<(std::ostream& stream, const Map& map)
{
    stream << map.width << ' ' << map.height << std::endl;

    for (unsigned int y = 0; y < map.height; y++)
    {
        for (unsigned int x = 0; x < map.width; x++)
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

std::istream &operator>>(std::istream& stream, Map& map)
{
    unsigned int width_, height_;
    stream >> width_ >> height_;

    if (stream.fail() || height_ == 0 || width_ == 0)
    {
        std::cerr << "Bad map dimensions\n";
        return stream;
    }

    // Ignore end of first line
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string map_str, line;
    for (unsigned int line_count = 0; line_count < height_; line_count++)
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

    // Create the map from the string representing it
    std::transform(
        std::begin(map_str), std::end(map_str),
        std::back_inserter(map.cells),
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

    map.width = width_;
    map.height = height_;

    return stream;
}
