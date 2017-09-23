/**
 * \file map.hpp
 * \brief Representation of a map.
 */

#pragma once

#include <vector>


/**
 * \brief Static information about the behaviour of the cell.
 */
enum class CellType
{
    Empty, ///< A cell that can't contain anything.
    Floor, ///< A cell you can walk on.
    Wall ///< A physical separation.
};


/**
 * \brief Represents an entire level.
 *
 * The map is of a fixed shape.
 * It describes its cells but also what entities are putted on it.
 */
class Map
{
public:
    /**
     * \brief Creates a map full of floor.
     */
    Map(int width, int height);

    /**
     * \brief Return the cell at coordinates x, y
     */
    CellType& tileAt(int x, int y);

private:
    int width;  ///< The width of the map.
    int height; ///< The height of the map.

    /**
     * \brief The type of each cell.
     */
    std::vector<CellType> cells;
};
