/**
 * \file generation/room.hpp
 * \brief Functions generating rooms patterns.
 */

#pragma once

#include <cmath>
#include <tuple>

#include "../rand.hpp"

#include "pattern.hpp"


/**
 * \brief Generate the pattern for a rectangular room.
 * \param width Width of the room.
 * \param height Height of the room.
 * \return The pattern of the room.
 * \note The rectangle will be centered on (0, 0)
 */
Pattern generate_rectangle(int width, int height);

/**
 * \brief Generate the pattern for a rectangular room.
 * \param size An approximate surface of the room.
 * \return The pattern of the room.
 * \note The rectangle will be centered on (0, 0)
 *
 * This will randomly pickup a width and height so that the surface is roughly size.
 */
Pattern generate_rectangle(int size);

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
 * \brief Generate a hallway from point cell1 to cell2.
 * \param cell1 Coordinates of first extremity of the path.
 * \param cell2 Coordinates of second extremity of the path.
 * \return The pattern corresponding to a path linking cell1 to cell2 if the cell (0, 0) of the pattern is placed on cell1.
 */
Pattern generate_hallway(std::pair<int, int> cell1, std::pair<int, int> cell2);

/**
 * \brief Add cells to a pattern so that it looks cave styled.
 * \param pattern The pattern to modify into cave styled.
 * \param nb_additions The number of cells to add to the pattern.
 */
void cavestyle_patch(Pattern& pattern, int nb_additions);

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
 * \brief Creates monsters to place on the room.
 * \param room The pattern we want to place monsters on.
 * \param load Number of monsters per 100 units of space (must be < 100).
 * \return A vector containing monsters we want to add.
 */
std::vector<std::shared_ptr<Entity>> place_monsters(const Pattern& room, float load);


/**
 * \brief Changes position of entities according to their room's position.
 * \param positions Center position of rooms.
 * \param entities For each room, the list of entities placed on it.
 * \return The vector containing all entities.
 * \warning The entities of parameter arrays are not copied.
 */
std::vector<std::shared_ptr<Entity>> merged_entities(
   const std::vector<std::pair<int, int>>& positions,
   std::vector<std::vector<std::shared_ptr<Entity>>>& entities
);
