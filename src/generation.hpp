/**
 * \file generation.hpp
 * \brief Algorithms and structures related to map generation.
 */

#pragma once

#include <set>
#include <tuple>
#include <vector>
#include <limits>
#include <algorithm>

#include "rand.hpp"
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
    int nb_rooms; ///< number of rooms to create on the map
    int room_min_size; ///< minimum number of cells contained on a room
    int room_max_size; ///< maximum number of cells contained on a room
};


/**
 * \brief Minimum x coordinate of cells in the pattern.
 * \attr pattern A pattern.
 * \return The minimum of x coordinates.
 */
int pattern_min_x(const Pattern& pattern);

/**
 * \brief Maximum x coordinate of cells in the pattern.
 * \attr pattern A pattern.
 * \return The maximum of x coordinates.
 */
int pattern_max_x(const Pattern& pattern);

/**
 * \brief Minimum y coordinate of cells in the pattern.
 * \attr pattern A pattern.
 * \return The minimum of y coordinates.
 */
int pattern_min_y(const Pattern& pattern);

/**
 * \brief Maximum y coordinate of cells in the pattern.
 * \attr pattern A pattern.
 * \return The maximum of y coordinates.
 */
int pattern_max_y(const Pattern& pattern);

/**
 * \brief Translate a pattern to remove negative coordinates.
 *
 * This will also align the pattern on top left.
 */
Pattern normalized_pattern(Pattern& pattern);

/**
 * \brief Merge patterns, placing them around given positions
 * \param positions An array of position where the ith position is the position of the map aligned with the position (0, 0) of the pattern
 * \param patterns An array of patterns
 * \return A set of every coordinates inside of the merged pattern
 *
 * \warning The new pattern can still contain negative coordinates.
 */
Pattern merged_patterns(
    const std::vector<std::pair<int, int>>& positions,
    const std::vector<Pattern>& patterns);

/**
 * \brief Generate a map corresponding to a floor pattern
 * \param pattern The cells included in the map as floors
 * \param width The with of the map (it means the pattern might be truncated)
 * \param height The height of the map (it means the pattern might be truncated)
 * \return A map where the pattern is surrounded by walls
 */
Map map_of_pattern(const Pattern& pattern, int width, int height);


/**
 * \brief Generate a hallway from point cell1 to cell2.
 * \param cell1 Coordinates of first extremity of the path.
 * \param cell2 Coordinates of second extremity of the path.
 * \return The pattern corresponding to a path linking cell1 to cell2 if the cell (0, 0) of the pattern is placed on cell1.
 */
Pattern make_hallway(std::pair<int, int> cell1, std::pair<int, int> cell2);

/**
 * \brief Contain a cave containing a fixed number of cells
 * \param size The number of cell the room contains
 * \return A set containing cells of the cave
 *
 * The room will be randomized. It will be centered on coordinate (0, 0).
 * \note If the size is less than 1, the room will still be of size 1.
 * \warning Some coordinates will be negative.
 */
Pattern generate_cave(int size);

/**
 * \brief Generate an entire level
 * \param mode Parameters used to generate the map
 * \return A pair containing the map, and the entities places at the start of the level
 */
std::pair<Map, std::vector<Entity>> generate(const GenerationMode &mode);
