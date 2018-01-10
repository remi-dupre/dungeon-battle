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

std::vector<std::pair<int, int>> Map::getChunks() const
{
    std::vector<std::pair<int, int>> ret;

    for (const auto& item: chunks)
        ret.push_back(item.first);

    return ret;
}

CellType& Map::cellAt(int x, int y)
{
    std::pair<int, int> chunk_id = Chunk::sector(x, y);
    std::pair<int, int> relt_pos = Chunk::relative(x, y);

    // Create a new chunk if needed
    assert(hasCell(x, y));

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

std::ostream& operator<<(std::ostream& stream, const Map& map)
{
    int size = map.chunks.size();
    stream.write(reinterpret_cast<const char*>(&size), sizeof(int));

    for (const auto& item : map.chunks) {
        stream.write(reinterpret_cast<const char*>(&item.first), sizeof(std::pair<int, int>));
        stream.write(reinterpret_cast<const char*>(&item.second), sizeof(Chunk));
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, Map& map)
{
    int size;
    stream.read(reinterpret_cast<char*>(&size), sizeof(int));

    for (int i = 0 ; i < size ; i++)
    {
        std::pair<int, int> chunk_id;
        Chunk chunk_ct;

        stream.read(reinterpret_cast<char*>(&chunk_id), sizeof(std::pair<int, int>));
        stream.read(reinterpret_cast<char*>(&chunk_ct), sizeof(Chunk));

        map.chunks[chunk_id] = chunk_ct;
    }

    return stream;
}
