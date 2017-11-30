/**
 * \file map.hpp
 * \brief Representation of maps.
 */

#pragma once

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <tuple>
#include <map>
#include <string>

#include "entity.hpp"
#include "utility.hpp"


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
 * \brief Represents a section of the map.
 */
class Chunk
{
public:
    constexpr static int SIZE = 32; ///< width and height of the chunks.

    /**
     * \brief Create a chunk full of empty cells.
     */
    Chunk();

    /**
     * \brief Get a read-write access to a cell by its coordinates
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return Reference to the cell
     * `x` and `y` must be in the ranges `[0, SIZE)` and `[0, SIZE)`
     */
    CellType& cellAt(int x, int y);

    /**
     * \brief Get a read only access to a cell by its coordinates
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return The cell type of the cell
     * `x` and `y` can be outside the ranges `[0, SIZE)` and `[0, SIZE)`
     * in which case CellType::Empty is returned
     */
    CellType cellAt(int x, int y) const;

    /**
     * \brief Get the coordinates of the chunk which contains a given cell.
     * \param x X coordinate of the cell in the map
     * \param y Y coordinate of the cell in the map
     * \return A pair containing the coordinates of the chunk.
     */
    static std::pair<int, int> sector(int x, int y);

    /**
     * \brief Get the coordinates corresponding to a given cell, in its chunk.
     * \param x X coordinate of the cell in the map
     * \param y Y coordinate of the cell in the map
     * \return A pair containing the coordinates of the chunk.
     */
    static std::pair<int, int> relative(int x, int y);

private:
    std::array<CellType, SIZE*SIZE> cells; ///< The type of each cells in the chunk
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
     * \brief Create an empty map
     */
    Map();

    /**
     * \brief Set a chunk.
     * \param x x-coordinate of the chunk.
     * \param y y-coordinate of the chunk.
     * \param chunk The chunk we want to add.
     */
    void setChunk(int x, int y, const Chunk& chunk);

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
     * \brief Check wether a chunk is in the generated part of the map.
     * \param x The x-coordinate of the chunk we are interested in.
     * \param y The y-coordinate of the chunk we are interested in.
     * \return true if the chunk containing exists.
     */
    bool hasChunk(int x, int y) const;

    /**
     * \brief Check wether a point is in the generated part of the map.
     * \param x The x-coordinate of the cell we are interested in.
     * \param y The y-coordinate of the cell we are interested in.
     * \return true if a chunk containing the cell exists.
     */
    bool hasCell(int x, int y) const;

    /**
     * \brief Get a read-write access to a cell by its coordinates
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return Reference to the cell
     * \note The chunk must be specified.
     */
    CellType& cellAt(int x, int y);

    /**
     * \brief Get a read-write access to a cell by its coordinates
     * \param coords Coordinates of the cell
     * \return Reference to the cell
     * \note The chunk must be specified.
     */
    CellType& cellAt(sf::Vector2i coords);

    /**
     * \brief Get a read only access to a cell by its coordinates
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return The cell type of the cell
     * \note If the chunk is not specified, returns CellType::Empty.
     */
    CellType cellAt(int x, int y) const;

    /**
     * \brief Get a read only access to a cell by its coordinates
     * \param coords Coordinates of the cell
     * \return The cell type of the cell
     * \note If the chunk is not specified, returns CellType::Empty.
     */
    CellType cellAt(sf::Vector2i coords) const;

    /**
     * \brief Check if there is a wall next to a cell
     */
    bool wallNext(int x, int y) const;

    /**
     * \brief Check if there is a wall next to a cell
     */
    bool wallNext(sf::Vector2i coords) const;

private:
    /**
     * \brief The content of each chunks
     */
    std::map<std::pair<int, int>, Chunk> chunks;
};
