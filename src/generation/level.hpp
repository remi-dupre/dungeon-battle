/**
 * \file generation/level.hpp
 * \brief Algorithms to generate an entire lvl.
 */

#pragma once

#include <queue>
#include <tuple>
#include <utility>
#include <vector>

#include "../map.hpp"
#include "../entity.hpp"
#include "../rand.hpp"

#include "gen_pattern.hpp"
#include "pattern.hpp"
#include "room.hpp"


/**
 * \brief A level, with the map and the entities
 */
struct Level
{
    Map map;
    std::vector<std::shared_ptr<Entity>> entities;
};


/**
 * \brief Gives a proposition of linkage between rooms.
 * \param positions The middle position of each room.
 * \param rooms The rooms we want to link.
 * \return The list of edges we want to add.
 */
std::vector<std::pair<size_t, size_t>> covering_paths(const std::vector<Room>& rooms);


/**
 * \brief Generate a map corresponding to a floor pattern.
 * \param pattern The cells included in the map as floors.
 * \return A map where the pattern is surrounded by walls.
 */
Map map_of_pattern(const Pattern& pattern);


/**
 * \brief Generate an entire level
 * \param mode Parameters used to generate the map
 * \return A pair containing the map, and the entities places at the start of the level
 */
Level generate(const GenerationMode &mode);
