#include <cassert>

#include "map.hpp"


/* Implementation of the class Chunk */

Chunk::Chunk()
{
    cells.fill(CellType::Empty);
}

CellType& Chunk::cellAt(int x, int y)
{
    assert(x >= 0 && x < SIZE);
    assert(y >= 0 && y < SIZE);

    return cells[x + SIZE * y];
}

CellType Chunk::cellAt(int x, int y) const
{
    assert(x >= 0 && x < SIZE);
    assert(y >= 0 && y < SIZE);

    return cells[x + SIZE * y];
}

/* Implementation of the class Map */

Map::Map() :
    width(0), height(0)
{}

Map::Map(int width, int height) :
    // Copy redundant information about the shape
    width(width), height(height)
{
    assert (width > 0);
    assert (height > 0);
}

void Map::set_chunk(int x, int y, const Chunk& chunk)
{
    chunks[{x, y}] = chunk;
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

bool Map::generated(int x, int y) const
{
    std::pair<int, int> chunk_id = std::make_pair(x, y) / Chunk::SIZE;
    return chunks.find(chunk_id) != end(chunks);
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
    assert(generated(x, y));

    std::pair<int, int> chunk_id = std::make_pair(x, y) / Chunk::SIZE;
    std::pair<int, int> relt_pos = std::make_pair(x, y) % Chunk::SIZE;

    return chunks[chunk_id].cellAt(relt_pos.first, relt_pos.second);
}

CellType& Map::cellAt(sf::Vector2i coords)
{
    return cellAt(coords.x, coords.y);
}

CellType Map::cellAt(int x, int y) const
{
    if (!generated(x, y))
    {
        return CellType::Empty;
    }
    else
    {
        std::pair<int, int> chunk_id = std::make_pair(x, y) / Chunk::SIZE;
        std::pair<int, int> relt_pos = std::make_pair(x, y) % Chunk::SIZE;

        return chunks.at(chunk_id).cellAt(relt_pos.first, relt_pos.second);
    }
}

CellType Map::cellAt(sf::Vector2i coords) const
{
    return cellAt(coords.x, coords.y);
}

bool Map::wallNext(int x, int y) const
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (cellAt(x + i, y + j) == CellType::Wall)
                return true;
        }
    }

    return false;
}

bool Map::wallNext(sf::Vector2i coords) const
{
    return wallNext(coords.x, coords.y);
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

    map.chunks.clear();

    // // Loads the map from the string representing it
    // std::transform(
    //     std::begin(map_str), std::end(map_str),
    //     std::back_inserter(map.chunks),
    //     [](const char& c) -> CellType
    //     {
    //         switch (c)
    //         {
    //             case '#':
    //                 return CellType::Wall;
    //                 break;
    //             case '.':
    //                 return CellType::Floor;
    //                 break;
    //             default:
    //                 return CellType::Empty;
    //                 break;
    //         }
    //     }
    // );

    map.width = width_;
    map.height = height_;

    return stream;
}

Map& Map::operator=(Map&& other)
{
    width = other.width;
    height = other.height;
    chunks = std::move(other.chunks);
    return *this;
}
