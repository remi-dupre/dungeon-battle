/**
* \file ai.hpp
* \brief Decide of the action of the monster.
*/

#pragma once

#include <cassert>
#include <cmath>
#include <limits>
#include <map>
#include <memory>
#include <queue>
#include <tuple>

#include "control.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "math.hpp"
#include "rand.hpp"
#include "utility.hpp"


struct Action;

/**
 * \brief Return weather a cell was seen according to the matrix seen, and say that now it has been seen.
 * \param seen The matrix saving weather a cell was seen.
 * \param position the cell we check.
 * \param startposition the cell representing the middle of the matrix seen.
 * \param sight the radius of the matrix from the start position.
 */
bool cell_seen(std::vector<std::vector<bool>> &seen, sf::Vector2i position, sf::Vector2i startposition, int sight);

/**
 * \brief Modify the matrix seen to indicate there is a entity on certain cells.
 * \param seen The matrix saving weather a cell was seen.
 * \param entities The list of entities on the map.
 * \param startposition the cell representing the middle of the matrix seen.
 * \param sight the radius of the matrix from the start position.
 */

void thereisaobstacle(const std::vector<std::shared_ptr<Entity>>& entities, std::vector<std::vector<bool>> &seen, sf::Vector2i startposition, int sight);

/**
 * \brief Decide of the action of the monster by the computation of a BFS algorithm.
 * \param monster The monster entity that is acting.
 * \param entities The list of entities on the map.
 * \param map The map.
 */

Action bfs_monster(const Character& monster, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map);

/**
 * \brief Decide of the action of the monster.
 * \param monster The monster entity that is acting.
 * \param entities The list of entities on the map.
 * \param map The map.
 */

Action get_input_monster(const Character& monster, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map);


