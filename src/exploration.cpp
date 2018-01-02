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

    auto [it, insertion] = indices.emplace(chunk, next_indice);
    if (insertion)
    {
        next_indice++;
        chunks.emplace_back();
    }

    chunks[it->second][chunk_pos.x + chunk_size * chunk_pos.y] = explored;
}


bool MapExploration::save(const std::string& path) const
{

}

bool MapExploration::load(const std::string& path)
{

}
