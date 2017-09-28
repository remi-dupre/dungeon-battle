/**
 * \file map.hpp
 * \brief Representation of maps.
 */

#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "entity.hpp"


/**
 * \brief Static information about the behaviour of the cell
 */
enum class CellType
{
    Empty, ///< A cell that can't contain anything
    Floor, ///< A cell you can walk on
    Wall ///< A physical separation
};


/**
 * \brief Represents an entire level
 *
 * The map is of a fixed shape.
 * It describes its cells but also what entities are on it.
 */
class Map
{
public:

    /**
     * \brief Create a map full of floor
     * \param width The width of the map
     * \param height The height of the map
     */
    explicit Map(unsigned int width, unsigned int height);

    /**
     * \brief Load the map from a file
     * \param filename Path of the file
     */
    bool loadFromFile(const std::string& filename);

    /**
     * \brief Save the map to a file
     * \param filename Path of the file
     */
    void saveToFile(const std::string& filename) const;

    /**
     * \brief Get the width of the map
     */
    int getWidth() const;

    /**
     * \brief Get the heigth of the map
     */
    int getHeight() const;

    /**
     * \brief Get a read-write access to a tile by its coordinates
     * \param x X coordinate of the tile
     * \param y Y coordinate of the tile
     * \return Reference to the tile
     *
     * `x` and `y` must be in the ranges `[0, map.width)` and `[0, map.heigth)`
     */
    CellType& cellAt(unsigned int x, unsigned int y);

    /**
     * \brief Get a read only access to a tile by its coordinates
     * \param x X coordinate of the tile
     * \param y Y coordinate of the tile
     * \return Const reference to the tile
     *
     * `x` and `y` must be in the ranges `[0, map.width)` and `[0, map.heigth)`
     */
    const CellType& cellAt(unsigned int x, unsigned int y) const;


private:

    unsigned int width;  ///< The width of the map
    unsigned int height; ///< The height of the map

    /**
     * \brief The type of each cell
     */
    std::vector<CellType> cells;

    /**
     * \brief A vector containing every entities availables on the map
     */
    std::vector<Entity> entities;

    friend std::ostream &operator<<(std::ostream&, const Map&);
    friend std::istream &operator>>(std::istream&, Map&);
};

/**
 * \brief `operator<<` overload for `Map`
 *
 * Overloads `operator<<` to output maps to streams
 */
std::ostream &operator<<(std::ostream& stream, const Map& map);

/**
 * \brief `operator>>` overload for `Map`
 *
 * Overloads `operator>>` to read maps from streams
 */
std::istream &operator>>(std::istream& stream, Map& map);
