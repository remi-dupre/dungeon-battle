#include <fstream>

#include "exploration.hpp"

#include "math.hpp"


bool MapExploration::isExplored(sf::Vector2i position)
{
    sf::Vector2i chunk = math::divide_floor(position, chunk_size);
    sf::Vector2u chunk_pos = static_cast<sf::Vector2<unsigned int>>
        (math::remainder(position, chunk_size));

    auto it = indices.find(chunk);
    if (it == indices.end())
        return false;

    return chunks[it->second][chunk_pos.x + chunk_size * chunk_pos.y];
}

void MapExploration::setExplored(sf::Vector2i position, bool explored)
{
    sf::Vector2i chunk = math::divide_floor(position, chunk_size);
    sf::Vector2u chunk_pos = static_cast<sf::Vector2<unsigned int>>
        (math::remainder(position, chunk_size));

    auto result = indices.emplace(chunk, chunks.size());
    auto it = result.first;
    bool insertion = result.second;

    if (insertion)
        chunks.emplace_back();

    chunks[it->second][chunk_pos.x + chunk_size * chunk_pos.y] = explored;
}


bool MapExploration::save(const std::string& path) const
{
    std::ofstream file {path, std::ios::trunc | std::ios::binary};

    uint32_t size = chunks.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));

    for (auto chunk_coords : indices)
    {
        sf::Vector2i coords = chunk_coords.first;
        file.write(reinterpret_cast<char*>(&coords.x), sizeof(int32_t));
        file.write(reinterpret_cast<char*>(&coords.y), sizeof(int32_t));

        const auto& chunk = chunks[chunk_coords.second];
        for (unsigned int i = 0; i + 7 < chunk_size * chunk_size; i += 8)
        {
            uint8_t byte =
                chunk[i    ] << 0 |
                chunk[i + 1] << 1 |
                chunk[i + 2] << 2 |
                chunk[i + 3] << 3 |
                chunk[i + 4] << 4 |
                chunk[i + 5] << 5 |
                chunk[i + 6] << 6 |
                chunk[i + 7] << 7;

            file.write(reinterpret_cast<char*>(&byte), sizeof(uint8_t));
        }
    }

    return true;
}

bool MapExploration::load(const std::string& path)
{
    std::ifstream file {path, std::ios::binary};

    uint32_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));

    for (unsigned int i_chunk = 0; i_chunk < size; ++i_chunk)
    {
        sf::Vector2i coords {};
        file.read(reinterpret_cast<char*>(&coords.x), sizeof(int32_t));
        file.read(reinterpret_cast<char*>(&coords.y), sizeof(int32_t));

        indices.emplace(coords, chunks.size());
        chunks.emplace_back();

        auto& chunk = chunks.back();
        for (unsigned int i = 0; i + 7 < chunk_size * chunk_size; i += 8)
        {
            uint8_t byte = 0;
            file.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t));

            chunk.set(i + 0, (byte & 0x01) != 0);
            chunk.set(i + 1, (byte & 0x02) != 0);
            chunk.set(i + 2, (byte & 0x04) != 0);
            chunk.set(i + 3, (byte & 0x08) != 0);
            chunk.set(i + 4, (byte & 0x10) != 0);
            chunk.set(i + 5, (byte & 0x20) != 0);
            chunk.set(i + 6, (byte & 0x40) != 0);
            chunk.set(i + 7, (byte & 0x80) != 0);
        }
    }

    return true;
}
