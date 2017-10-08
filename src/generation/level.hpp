/**
 * \file generation/level.hpp
 * \brief Algorithms to generate an entire lvl.
 */

#pragma once

#include <tuple>
#include <vector>

#include "../map.hpp"
#include "../entity.hpp"
#include "../rand.hpp"

#include "pattern.hpp"
#include "room.hpp"


/**
 * \brief A level, with the map and the entities
 */
typedef std::pair<Map, std::vector<std::shared_ptr<Entity>>> Level;

/**
 * \brief Describe the general design of the level.
 */
enum class LevelType
{
    Flat, ///< Rectangular-shaped rooms
    Cave ///< Cave shaped rooms
};

/**
 * \brief Parameters on how to generate the level
 */
struct GenerationMode
{
    int nb_rooms; ///< number of rooms to create on the map
    int room_min_size; ///< minimum number of cells contained on a room
    int room_max_size; ///< maximum number of cells contained on a room
    int room_margin; ///< minimum space added between two rooms

    float monster_load; ///< Number of monsters per 100 unit of space

    LevelType type; ///< Kind of design for the rooms
};


/**
 * \brief Modify rooms positioning to add spacing between them.
 * \param Positions center positions of the rooms.
 * \param Rooms patterns corresponding to th rooms.
 * \param Spacing minimum space needed between pair of cells of two differents patterns.
 */
void separate_rooms(
    std::vector<std::pair<int, int>>& positions,
    const std::vector<Pattern>& rooms,
    int spacing
);


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
