/**
 * \file generation.hpp
 * \brief Algorithms and structures related to map generation.
 */

#pragma once

#include <random>
#include <set>
#include <tuple>
#include <vector>

#include "entity.hpp"
#include "map.hpp"


/**
 * \brief The set of cells contained in a pattern
 */
typedef std::set<std::pair<int, int>> Pattern;


/**
 * \brief Parameters on how to generate the level
 */
struct GenerationMode
{
    int width; ///< width of the generated level
    int height; ///< height of the generated level

    int nb_rooms; ///< number of rooms to create on the map
    int room_size; ///< number of cells contained on a room
};


/**
 * \brief Contain a cave containing a fixed number of cells
 * \param size The number of cell the room contains
 * \return A set containing cells of the cave
 *
 * The room will be randomized. It will be centered on coordinate (0, 0).
 * \note If the size is less than 1, the room will still be of size 1.
 * \warning Some coordinates will be negative.
 */
Pattern generateCave(int size);


/**
 * \brief Merge patterns, placing them around given positions
 * \param positions An array of position where the ith position is the position of the map aligned with the position (0, 0) of the pattern
 * \param patterns An array of patterns
 * \return A set of every coordinates inside of the merged pattern
 * 
 * \warning The new pattern can still contain negative coordinates.
 */
Pattern mergePatterns(
    const std::vector<std::pair<int, int>>& positions,
    const std::vector<Pattern>& patterns);


/**
 * \brief Generate a map corresponding to a floor pattern
 * \param pattern The cells included in the map as floors
 * \param width The with of the map (it means the pattern might be truncated)
 * \param height The height of the map (it means the pattern might be truncated)
 * \return A map where the pattern is surrounded by walls
 */
Map mapOfPattern(const Pattern& pattern, int width, int height);


/**
 * \brief Generate an entire level
 * \param mode Parameters used to generate the map
 * \return A pair containing the map, and the entities places at the start of the level
 */
std::pair<Map, std::vector<Entity>> generate(const GenerationMode &mode);
