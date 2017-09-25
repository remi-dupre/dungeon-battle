/**
 * \file map.hpp
 * \brief Representation of a map.
 */

#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

#include "entity.hpp"


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
 * It describes its cells but also what entities are on it.
 */
class Map
{
public:
    /**
     * \brief Creates a map full of floor.
     */
    explicit Map(int width, int height);

    /**
     * \brief Return the width of the map
     */
    int getWidth() const;

    /**
     * \brief Return the heigth of the map
     */
    int getHeight() const;

    /**
     * \brief Get a read-write access to a cell by its coordinates
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return Reference to the cell
     * `x` and `y` must be in the ranges `[0, map.width)` and `[0, map.heigth)`
     */
    CellType& cellAt(int x, int y);

    /**
     * \brief Get a read only access to a cell by its coordinates
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return Const reference to the cell
     * `x` and `y` must be in the ranges `[0, map.width)` and `[0, map.heigth)`
     */
    const CellType& cellAt(int x, int y) const;

    /**
     * \brief Get the list of every entities on the map.
     * \return Const reference to a vector containing all entities of the map.
     * If two entities are on the same cell, the last one is over the first one.
     */
    const std::vector<Entity>& getEntities() const;

    /**
     * \brief Get all entities putted on a specific cell
     * \param position Position vector of the cell
     * \return A vector of copies of the entities putted on the cell.
     */
    std::vector<Entity> getEntitiesOnCell(sf::Vector2i position) const;

    /**
     * \brief Get all entities putted on a specific cell
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return A vector of copies of the entities putted on the cell.
     */
    std::vector<Entity> getEntitiesOnCell(int x, int y) const;

private:
    int width;  ///< The width of the map.
    int height; ///< The height of the map.

    /**
     * \brief The type of each cell.
     */
    std::vector<CellType> cells;

    /**
     * \brief A vector containing every entities availables on the map.
     */
    std::vector<Entity> entities;
};
