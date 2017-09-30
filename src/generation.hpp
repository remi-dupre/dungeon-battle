/**
 * \file generation.hpp
 * \brief Algorithms and structures related to map generation.
 */

#pragma once

#include <tuple>
#include <vector>
#include <set>
#include <random>

#include "map.hpp"
#include "entity.hpp"


/**
 * \brief Parameters on how to generate the level.
 */
struct GenerationMode
{
    int width; ///< width of the generated lvl
    int height; ///< height of the generated lvl

    int nb_rooms; ///< number of rooms to create on the map
    int rooms_size; ///< number of cells contained on a room
};

/**
 * \brief Generate an entire lvl.
 * \param mode Parameters used to generate the map.
 * \return A pair containing the map, and the entities places at the start of the lvl.
 */
std::pair<Map, std::vector<Entity>> generate(const GenerationMode &mode);
