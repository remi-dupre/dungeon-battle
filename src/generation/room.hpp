/**
 * \file generation/room.hpp
 * \brief Functions generating rooms patterns.
 */

#pragma once

#include <cassert>
#include <limits>
#include <tuple>

#include "../entity.hpp"
#include "../rand.hpp"

#include "gen_pattern.hpp"
#include "space.hpp"
#include "pattern.hpp"


/**
 * \brief Represent a single generated room.
 */
struct Room
{
    Point position; ///< Center position of the room.

    Pattern cells; ///< Relative positions of the cells placed on the room.
    Pattern nodes; ///< Cells of the pattern that can be used to enter it.

    std::vector<std::shared_ptr<Entity>> entities; ///< Entities placed on the room.

    /**
     * \brief Default constructor : empty room in (0, 0).
     */
    Room();
};


/**
 * \brief Translate a room to remove negative coordinates.
 * \param pattern A room that could have negative coordinates.
 * \return The newly aligned room.
 *
 * \note This will also align the room on top left : its position will be (0, 0).
 */
Room normalized_room(const Room& room);

/**
 * \brief Return a room as the union of all the rooms.
 * \attr rooms The list of rooms.
 * \return A room, containing all the entities, and all the cells.
 * \warning This operation erases all the nodes.
 */
Room merged_rooms(const std::vector<Room>& rooms);

/**
 * \brief Modify rooms positioning to add spacing between them.
 * \param positions List of rooms we want to space.
 * \param spacing minimum space needed between pair of cells of two differents patterns.
 */
void separate_rooms(std::vector<Room>& rooms, int spacing);

/**
 * \brief Find a pair of closest nodes between two rooms.
 * \param room1 A room, where nodes are specified.
 * \param room2 A room, where nodes are specified.
 * \param A pair of the coordinates of the nodes.
 */
std::pair<Point, Point> closest_nodes(const Room& room1, const Room& room2);

/**
 * \brief Calculates node to node distance of two rooms.
 * \param room1 A room, where nodes are specified.
 * \param room2 A room, where nodes are specified.
 * \param The distance between the two closest nodes of the room.
 */
int ntn_dist(const Room& room1, const Room& room2);


/**
 * \brief Creates monsters to place on the room.
 * \param room The room we want to add monsters to.
 * \param load Number of monsters per 100 units of space (must be < 100).
 * \return A vector containing monsters we want to add.
 */
void add_monsters(Room& room, float load);


/**
 * \brief Generate a labyrinth room.
 * \param width The width of the maze. (must be odd)
 * \param height The height of the maze. (must be odd)
 * \return A room containing a maze, with relevant nodes.
 */
Room maze_room(int width, int height);
