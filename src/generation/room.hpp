/**
 * \file generation/room.hpp
 * \brief Functions generating rooms patterns.
 */

#pragma once

#include <tuple>

#include "../entity.hpp"
#include "../rand.hpp"

#include "pattern.hpp"


/**
 * \brief Represent a single generated room.
 */
struct Room
{
    std::pair<int, int> position; ///< Center position of the room.
    Pattern cells; ///< Relative positions of the cells placed on the room.
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
 */
Room merged_rooms(const std::vector<Room>& rooms);

/**
 * \brief Modify rooms positioning to add spacing between them.
 * \param positions List of rooms we want to space.
 * \param spacing minimum space needed between pair of cells of two differents patterns.
 */
void separate_rooms(std::vector<Room>& rooms, int spacing);

/**
 * \brief Creates monsters to place on the room.
 * \param room The room we want to add monsters to.
 * \param load Number of monsters per 100 units of space (must be < 100).
 * \return A vector containing monsters we want to add.
 */
void add_monsters(Room& room, float load);
