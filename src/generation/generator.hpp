/**
 * \file   generation/generator.hpp
 * \brief  Define a class allowing to generate an entire level.
 */

#pragma once

#include <queue>
#include <set>
#include <tuple>
#include <vector>

#include "pattern.hpp"
#include "room.hpp"
#include "space.hpp"

#include "../map.hpp"
#include "../entity.hpp"


// Number of chunks that will be generated on border of requested chunks
constexpr int GEN_BORDER = 2;

/**
 * \brief A level, with the map and the entities
 */
struct Level
{
    Map map;
    std::vector<std::shared_ptr<Entity>> entities;
};

/**
 * \brief  An object that can generate chunks of the map.
 *
 * The generator can answer to chunk requests :
 *  - given a coordinate (not necessary positive), get the map
 *  - given a coordinate (not necessary positive), get the entities
 */
class Generator
{
public:
    /**
     * \brief    Main constructor. Initialisation of parameters.
     *
     * \param    parameters The configuration of the map generation.
     * \param    seed The seed used for the generation.
     * \warning  Note that we may need to use the generator exacly with the same queries to get the same map.
     */
    Generator(const GenerationMode& parameters, int seed);

    /**
     * \brief   Get the chunk of coordinates (x, y).
     * \param   x x-coordinate of the chunk.
     * \param   y y-coordinate of the chunk.
     * \return  The content of the queried chunk.
     */
    Chunk getChunkCells(int x, int y);

    /**
     * \brief Get the enties initially placed on the chunk of coordinates (x, y).
     * \param   x x-coordinate of the chunk.
     * \param   y y-coordinate of the chunk.
     * \return  The list of the entities initially placed on the queried chunk.
     */
    std::vector<std::shared_ptr<Entity>> getChunkEntities(int x, int y);

    /**
     * \brief  Indicate to generate around a chunk.
     * \param  y       y-coordinate of the chunk.
     * \param  x       x-coordinate of the chunk.
     * \param  radius  The radius of a square of center {x, y}.
     * \note   This function might only be used for infinite generation since the whole map is generated at creation for finite generation.
     *
     * It means that the generated square has a diagonal from {x-radius, y-radius} to {x+radius, y+radius}.
     * It won't generate a chunk if it has already been generated.
     */
    void generateRadius(int x, int y, int radius);


private:
    ///< Parameters for the generation
    GenerationMode parameters;

    ///< Seed used for the generation of the map
    int seed;

    ///< Set of chunk we don't wan't to modify anymore
    std::set<std::pair<int, int>> locked;

    ///< Set of chunks that have already been built so far
    std::set<std::pair<int, int>> built;

    ///< Order in which chunks have been generated
    std::vector<std::pair<int, int>> build_order;

    ///< List of rooms generated so far
    std::vector<Room> rooms;

    ///< A cached version of the map we generated so far.
    Map cachedMap;

    ///< Keep track of connections between rooms
    std::set<std::pair<size_t, size_t>> room_links;

    /**
     * \brief  Generate some rooms centered on given chunck.
     * \param  x x-coordinate of the center chunk.
     * \param  y y-coordinate of the center chunk.
     * \param  n number of rooms to generate.
     */
    void addRooms(int x, int y, int n);

    /**
     * \brief  Ensure connexity of the level.
     * Update room_links and create new hallways between rooms.
     */
    void updateLinks();

    /**
     * \brief  Specify that a room has been added to the map.
     * \param  room The index of the room in `rooms`
     * This method must be called each time a room is added to the map.
     */
    void registerRoom(size_t room);
};
