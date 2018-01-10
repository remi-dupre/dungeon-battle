/**
 * \file generation/room.hpp
 * \brief Functions generating rooms patterns.
 */

#pragma once

#include <cassert>
#include <limits>
#include <tuple>

#include "../entity.hpp"
#include "../rand.hpp"

#include "gen_pattern.hpp"
#include "pattern.hpp"
#include "space.hpp"


/**
 * \brief  Represent a single generated room.
 *
 * \section Role
 *   The goal of a room is to encapsulate all the informations a room can contain, but to still easily be moved on the map during the generation.
 *   A room contains informations about its cells and the entities it initially contains
 *
 * \section Behaviour
 *   Actually a room represents only the floor cells it contains, walls will be added later. Theses cells are represented by a <b>set of their coordinates relative to the room position</b>. Thus, a room can be moved by only changing their position attribute.
 *   In the same way, entities are placed relatively to the room position.
 *   A few optimisations have also been done to efficiently check the distance between two rooms (cf. KDTree::).
 */
class Room
{
public:
    /**
     * \brief  Copy a room.
     */
    Room(const Room&) = default;

    /**
     * \brief  Default constructor place a room at (0, 0).
     */
    Room(const Pattern& cells);

    /**
     * \brief   Get room's position.
     * \return  The central position of the room.
     */
    Point getPosition() const;

     /**
      * \brief  Set room's position.
      * \param  position  The new position of the center of the room.
      */
    void setPosition(const Point& nposition);

    /**
     * \brief    Get the set of cells of the room.
     * \return   The set of cells of the room, relatively to its position.
     * \warning  This will make a copy of all cells of the structure.
     */
    Pattern getCells() const;

    /**
     * \brief   Get the size of the room.
     * \return  The number of cells in the room.
     */
    int size() const;

    /**
     * \brief   Check if the room contains the given cell.
     * \param   cell  A cell we want to check (position on the map).
     * \return  true if cell is a cell of the map that is in this room.
     */
    bool hasCell(const Point& cell) const;

    /**
     * \brief   Get a set of points that can be used to enter the room.
     * \return  The set of nodes entering the room, relatively to its position.
     */
    Pattern getNodes() const;

    /**
     * \brief   Override room's nodes.
     */
    void setNodes(const Pattern& nnodes);

    /**
     * \brief   Get the set of entities placed in the room.
     * \return  A vector of pointer of entities.
     */
    std::vector<std::shared_ptr<Entity>> getEntities() const;

    /**
     * \brief  Add an entity on the map.
     */
    void addEntity(std::shared_ptr<Entity> entity);

    /**
     * \brief   Check wether two rooms are at least spaced of a given distance.
     * \param   room1    The first room we want to compare.
     * \param   room2    The second room we want to compare.
     * \param   spacing  The spacing we want between the two rooms.
     * \return  true if each cell of the first room are at least at distance spacing from cells of room2.
     */
    friend bool spaced(const Room& room1, const Room& room2, int spacing);

private:
    Point position; ///< Center position of the room.

    Pattern cells; ///< Relative positions of the cells placed on the room.
    Pattern nodes; ///< Cells of the pattern that can be used to enter it.

    KDTree treeCells; ///< Alternative representation of cells

    std::vector<std::shared_ptr<Entity>> entities; ///< Entities placed on the room.
};

/**
 * \brief  Modify rooms with index within a range repositioning to add spacing between them.
 * \param  positions  List of rooms we want to space.
 * \param  spacing    Minimum space needed between pair of cells of two differents patterns.
 * \param  left       Index of the first room we are allowed to move.
 * \param  right      Index of the first room we are not allowed to move.
 */
void separate_rooms(std::vector<Room>& rooms, int spacing, size_t left, size_t right);

/**
 * \brief Find a pair of closest nodes between two rooms.
 * \param   room1  A room, where nodes are specified.
 * \param   room2  A room, where nodes are specified.
 * \return  A pair of the coordinates of the nodes.
 */
std::pair<Point, Point> closest_nodes(const Room& room1, const Room& room2);

/**
 * \brief   Calculates node to node distance of two rooms.
 * \param   room1  A room, where nodes are specified.
 * \param   room2  A room, where nodes are specified.
 * \return  The distance between the two closest nodes of the room.
 */
int ntn_dist(const Room& room1, const Room& room2);


/**
 * \brief   Creates monsters to place on the room.
 * \param   room  The room we want to add monsters to.
 * \param   load  Number of monsters per 100 units of space (must be < 100).
 * \return  A vector containing monsters we want to add.
 */
void add_monsters(Room& room, float load);


/**
 * \brief   Generate a labyrinth room.
 * \param   width   The width of the maze. (must be odd)
 * \param   height  The height of the maze. (must be odd)
 * \return  A room containing a maze, with relevant nodes.
 */
Room maze_room(int width, int height);
