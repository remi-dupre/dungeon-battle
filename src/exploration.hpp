#pragma once

#include <bitset>
#include <map>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "utility.hpp"


class MapExploration
{
public:
    MapExploration() = default;

    bool isExplored(sf::Vector2i position);
    void setExplored(sf::Vector2i position, bool explored = true);

    bool save(const std::string& path) const;
    bool load(const std::string& path);

    static constexpr int chunk_size = 32;

private:

    std::map<sf::Vector2i, size_t> indices;
    std::vector<std::bitset<chunk_size * chunk_size>> chunks;

    size_t next_indice = 0;
};
