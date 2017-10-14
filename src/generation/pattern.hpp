/**
 * \file generation/pattern.hpp
 * \brief Functions to deal with patterns.
 */

#pragma once

#include <cassert>
#include <set>
#include <tuple>
#include <vector>

#include "../entity.hpp"
#include "../utility.hpp"

#include "space.hpp"


/**
 * \brief The set of cells contained in a pattern
 */
typedef std::set<Point> Pattern;


/**
 * \brief Minimum x coordinate of cells in the pattern
 * \param pattern A pattern
 * \return The minimum of x coordinates
 */
int pattern_min_x(const Pattern& pattern);

/**
 * \brief Maximum x coordinate of cells in the pattern
 * \param pattern A pattern
 * \return The maximum of x coordinates
 */
int pattern_max_x(const Pattern& pattern);

/**
 * \brief Minimum y coordinate of cells in the pattern
 * \param pattern A pattern
 * \return The minimum of y coordinates
 */
int pattern_min_y(const Pattern& pattern);

/**
 * \brief Maximum y coordinate of cells in the pattern
 * \param pattern A pattern
 * \return The maximum of y coordinates
 */
int pattern_max_y(const Pattern& pattern);


/**
 * \brief Returns a pattern containing the surrounding of a pattern
 * \param pattern A filled pattern.
 * \return A pattern only containing the frontier with the outside.
 */
Pattern surrounding(const Pattern& pattern);


/**
 * \brief Check if a minimum space is assured between two rooms.
 * \param position1 Center position of the first room.
 * \param pattern1 Pattern of the first room.
 * \param position2 Center position of the second room.
 * \param pattern2 Pattern of the second room.
 * \param spacing The minimum distance between two cells of the rooms.
 * \return Returns true if any pair of cells are spaced of at least distance.
 */
bool spaced(
    Point position1, const Pattern& pattern1,
    Point position2, const Pattern& pattern2,
    int spacing
);

/**
 * \brief Check of a pattern is conflicting with another.
 * \param position1 Center position of the first room.
 * \param pattern1 Pattern of the first room.
 * \param position2 Center position of the second room.
 * \param pattern2 Pattern of the second room.
 * \return Returns true if the two patterns have a common cell.
 */
bool superposed(
    Point position1, const Pattern& pattern1,
    Point position2, const Pattern& pattern2
);

/**
 * \brief Merge patterns, placing them around given positions
 * \param positions An array of position where the ith position is the position of the map aligned with the position (0, 0) of the pattern
 * \param patterns An array of patterns
 * \return A set of every coordinates inside of the merged pattern
 *
 * \warning The new pattern can still contain negative coordinates.
 */
Pattern merged_patterns(
    const std::vector<Point>& positions,
    const std::vector<Pattern>& patterns
);
