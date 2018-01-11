/**
 * \file   generation/generator.hpp
 * \brief  Define a class allowing to generate an entire level.
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
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
 * \section Features
 *   The generator can answer to chunk requests :
 *    - given a coordinate (not necessary positive), get the map
 *    - given a coordinate (not necessary positive), get the entities
 *   It is also possible to ask to pregenerate a radius and to ask what are the already generated but not requested chunks.
 *
 * \section Behaviour
 *   The generator keeps up to date a list of rooms. Creating new rooms always follow the same pattern:
 *    - generate rooms shapes and place them at an arbitrary position
 *    - separate rooms to avoid them from collapsing
 *    - add hallways between rooms (it generates new rooms)
 *    - add entities on rooms
 *    - draw rooms on a cached map
 *   If the map has to be finite, theses steps will only be executed once, thus the generator will only have to answer using his cached map.
 */
class Generator
{
public:
    /**
     * \brief    Construct a generator doing nothing.
     */
    Generator();

    /**
     * \brief    Main constructor. Initialisation of parameters.
     *
     * \param    parameters The configuration of the map generation.
     * \warning  Note that we may need to use the generator exacly with the same queries to get the same map.
     */
    Generator(const GenerationMode& parameters);

    // a copy would need to restart a thread, and is very heavy
    Generator(Generator&&) = delete;
    Generator(const Generator&) = delete;

    /**
     * \brief   Get the chunk of coordinates (x, y).
     * \param   x x-coordinate of the chunk.
     * \param   y y-coordinate of the chunk.
     * \return  The content of the queried chunk.
     */
    Chunk getChunkCells(int x, int y);

    /**
     * \brief   Get the list of all cached chunks not requested so far.
     * \return  A vector of the id of each not-requested chunk.
     */
    std::vector<std::pair<int, int>> getCachedChunks();

    /**
     * \brief Get the enties initially placed on the chunk of coordinates (x, y).
     * \param   x x-coordinate of the chunk.
     * \param   y y-coordinate of the chunk.
     * \return  The list of the entities initially placed on the queried chunk.
     */
    std::vector<std::shared_ptr<Entity>> getChunkEntities(int x, int y);

    /**
     * \brief  Indicate to generate around a chunk.
     * \param  y         y-coordinate of the chunk.
     * \param  x         x-coordinate of the chunk.
     * \param  radius    The radius of a square of center {x, y}.
     * \param  priority  If set to true this chunk is generated before other chunks without priority.
     * \note   This function might only be used for infinite generation since the whole map is generated at creation for finite generation.
     *
     * It means that the generated square has a diagonal from {x-radius, y-radius} to {x+radius, y+radius}.
     * It won't generate a chunk if it has already been generated.
     */
    void preGenerateRadius(int x, int y, int radius, bool priority = false);

    /**
     * \brief  Same as preGenerateRadius, but the call will only end when the generation is over. Thus the priority used.
     */
    void generateRadius(int x, int y, int radius);


private:
    /**
     * \brief   Check if the cells of a chunk are locked.
     * \param   x x-coordinate of the chunk.
     * \param   y y-coordinate of the chunk.
     * \return  Wether the chunk could or couldn't change.
     */
    bool isLockedChunk(int x, int y);

    /**
     * \brief  Assert that a chunk must now be locked.
     * \param  x x-coordinate of the chunk.
     * \param  y y-coordinate of the chunk.
     */
    void setLockedChunk(int x, int y);

    /**
     * \brief   Check if rooms have already been added to the chunk.
     * \param   x x-coordinate of the chunk.
     * \param   y y-coordinate of the chunk.
     * \return  True if the chunk is already filled with rooms.
     */
    bool isFilledChunk(int x, int y);

    /**
     * \brief  Assert that a chunk has been filled.
     * \param  x x-coordinate of the chunk.
     * \param  y y-coordinate of the chunk.
     */
    void setFilledChunk(int x, int y);

    /**
     * \brief  Generate some rooms centered on given chunck.
     * \param  x x-coordinate of the center chunk.
     * \param  y y-coordinate of the center chunk.
     * \param  n number of rooms to generate.
     * \warn   This can generate less than n rooms.
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

    /**
     * \brief  Loop that generates any chunk given in the list.
     */
    void generationLoop();


    ///< Parameters for the generation
    GenerationMode parameters;

    ///< Thread that actualy generates
    std::thread generating_thread;

    ///< A list of chunks we need to generate
    // This list is read from its front, thus, most important tasks are put in the front
    std::list<std::pair<int, int>> to_generate;

    ///< Lock for to_generate
    std::mutex to_generate_lock;

    ///< Set of chunk we don't wan't to modify anymore
    std::set<std::pair<int, int>> locked;

    ///< Lock for locked
    std::mutex locked_lock;

    ///< Set of chunks that have already been built so far
    std::set<std::pair<int, int>> filled;

    ///< Lock for filled
    std::mutex filled_lock;

    ///< List of rooms generated so far
    std::vector<Room> rooms;

    ///< A cached version of the map we generated so far
    Map cached_map;

    ///< A cached version of entities for each chunk
    std::map<std::pair<int, int>, std::vector<std::shared_ptr<Entity>>> cached_entities;

    ///< Lock for the cache
    std::mutex cache_lock;

    ///< Keep track of connections between rooms
    std::set<std::pair<size_t, size_t>> room_links;

};
