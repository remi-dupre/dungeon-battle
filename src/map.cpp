#include <cassert>
#include <fstream>

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

std::pair<int, int> Chunk::sector(int x, int y) {
    if (x < 0) x -= SIZE - 1;
    if (y < 0) y -= SIZE - 1;

    return std::make_pair(x, y) / SIZE;
}

std::pair<int, int> Chunk::relative(int x, int y) {
    if (x < 0) x -= SIZE - 1;
    if (y < 0) y -= SIZE - 1;

    return std::make_pair(x, y) % SIZE;
}

/* Implementation of the class Map */

Map::Map() {}

void Map::setChunk(int x, int y, const Chunk& chunk)
{
    chunks[{x, y}] = chunk;
}

bool Map::hasCell(int x, int y) const
{
    std::pair<int, int> chunk_id = Chunk::sector(x, y);
    return hasChunk(chunk_id.first, chunk_id.second);
}

bool Map::hasChunk(int x, int y) const
{
    std::pair<int, int> chunk_id = {x, y};
    return chunks.find(chunk_id) != end(chunks);
}

CellType& Map::cellAt(int x, int y)
{
    std::pair<int, int> chunk_id = Chunk::sector(x, y);
    std::pair<int, int> relt_pos = Chunk::relative(x, y);

    // Create a new chunk if needed
    assert(hasCell(x, y));
    // if (!hasCell(x, y))
        // chunks[chunk_id] = Chunk();

    return chunks[chunk_id].cellAt(relt_pos.first, relt_pos.second);
}

CellType& Map::cellAt(sf::Vector2i coords)
{
    return cellAt(coords.x, coords.y);
}

CellType Map::cellAt(int x, int y) const
{
    if (!hasCell(x, y))
    {
        return CellType::Empty;
    }
    else
    {
        std::pair<int, int> chunk_id = Chunk::sector(x, y);
        std::pair<int, int> relt_pos = Chunk::relative(x, y);

        return chunks.at(chunk_id).cellAt(relt_pos.first, relt_pos.second);
    }
}

CellType Map::cellAt(sf::Vector2i coords) const
{
    return cellAt(coords.x, coords.y);
}

Chunk Map::chunkAt(int x, int y) const
{
    assert(chunks.find({x, y}) != end(chunks));

    return chunks.at({x, y});
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


bool Map::loadFromFile(const std::string& filename)
{
    std::ifstream file;
    file.open(filename);

    if (file.fail())
        return false;

    // file >> *this;

    return true;
}

void Map::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);

    if (!file.is_open())
        return;

    // file << *this;

    return;
}

std::ostream& operator<<(std::ostream& stream, const Map& map)
{
    // stream.write(reinterpret_cast<const char*>(&map.width), sizeof(int32_t));
    // stream.write(reinterpret_cast<const char*>(&map.height), sizeof(int32_t));
    //
    // stream.write(reinterpret_cast<const char*>(map.cells.data()), map.width * map.height);
    //
    // return stream;
}

std::istream& operator>>(std::istream& stream, Map& map)
{
    // int32_t width_, height_;
    // stream.read(reinterpret_cast<char*>(&width_), sizeof(int32_t));
    // stream.read(reinterpret_cast<char*>(&height_), sizeof(int32_t));
    //
    // if (stream.fail() || height_ <= 0 || width_ <= 0)
    // {
    //     std::cerr << "Bad map dimensions\n";
    //     return stream;
    // }
    //
    // std::vector<CellType> cells(width_ * height_, CellType::Empty);
    // stream.read(reinterpret_cast<char*>(cells.data()), width_ * height_);
    //
    // if (stream.fail())
    // {
    //     std::cerr << "Bad map data\n";
    //     return stream;
    // }
    //
    // map.width = width_;
    // map.height = height_;
    // std::swap(cells, map.cells);
    //
    // return stream;
}
