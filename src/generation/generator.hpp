/**
 * \file generation/generator.hpp
 * \brief Define a class allowing to generate an entire level.
 */

#pragma once

#include <set>
#include <tuple>
#include <vector>

#include "level.hpp"
#include "room.hpp"
#include "space.hpp"

#include "../map.hpp"
#include "../entity.hpp"


///< Describe the role of a room
enum class RoomType
{
    Regular, ///< A regular room
    Hallway  ///< A hallway generated to link two room
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

    /**
     * \brief Generate some rooms centered on given chunck.
     * \param x x-coordinate of the center chunk.
     * \param y y-coordinate of the center chunk.
     * \param n number of rooms to generate.
     */
    void addRooms(int x, int y, size_t n);


private:
    ///< Parameters for the generation
    GenerationMode parameters;

    ///< Seed used for the generation of the map
    int seed;

    ///< Set of chunks that have already been requested
    std::set<std::pair<int, int>> requested;

    ///< List of rooms generated so far
    std::vector<Room> rooms;

    ///< Links we have between the rooms
    std::vector<std::pair<size_t, size_t>> room_links;
};
