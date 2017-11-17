/**
 * \file generation/generator.hpp
 * \brief Define a class allowing to generate an entire level.
 */

#pragma once

#include <vector>

#include "../map.hpp"
#include "../entity.hpp"


/**
 * \brief Describe the general design of the level.
 */
enum class LevelType
{
    Flat, ///< Rectangular-shaped rooms
    Cave ///< Cave shaped rooms
};

/**
 * \brief Parameters on how to generate the level
 */
struct GenerationMode
{
    bool infinite; ///< Wether the map should be generated dynamically
    int nb_rooms; ///< number of rooms to create on the map (ignored if the map is infinite)

    int room_min_size; ///< minimum number of cells contained on a room
    int room_max_size; ///< maximum number of cells contained on a room
    int room_margin; ///< minimum space added between two rooms

    float monster_load; ///< Number of monsters per 100 unit of space

    float maze_density; ///< Proportion of rooms that are replaced with mazes
    LevelType type; ///< Kind of design for the rooms
};

/**
 * \brief An object that can generate chunks of the map.
 *
 * The generator can answer to chunk requests :
 *  - given a coordinate (not necessary positive), get the map
 *  - given a coordinate (not necessary positive), get the entities
 */
class Generator
{
public:
    /**
     * \brief Main constructor. Initialisation of parameters.
     *
     * \param parameters The configuration of the map generation.
     * \param seed The seed used for the generation.
     * \warning Note that we may need to use the generator exacly with the same queries to get the same map.
     */
    Generator(const GenerationMode& parameters, int seed);

    /**
     * \brief Get the chunk of coordinates (x, y).
     * \param x x-coordinate of the chunk.
     * \param y y-coordinate of the chunk.
     * \return The content of the queried chunk.
     */
    Chunk getChunkCells(int x, int y);

    /**
     * \brief Get the enties initially placed on the chunk of coordinates (x, y).
     * \param x x-coordinate of the chunk.
     * \param y y-coordinate of the chunk.
     * \return The list of the entities initially placed on the queried chunk.
     */
    std::vector<std::shared_ptr<Entity>> getChunkEntities(int x, int y);

private:
    GenerationMode parameters; ///< Parameters for the generation
    int seed; ///< Seed used for the generation of the map
};
