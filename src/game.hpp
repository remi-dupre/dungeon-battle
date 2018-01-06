/**
 * \file game.hpp
 * \brief Contains a class representing the game.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <map>
#include <memory>
#include <random>
#include <string>

#include <SFML/Graphics.hpp>

#include "generation/level.hpp"

#include "args.hpp"
#include "config.hpp"
#include "control.hpp"
#include "exploration.hpp"
#include "map.hpp"
#include "menu/menus.hpp"
#include "rand.hpp"
#include "render.hpp"
#include "utility.hpp"


/**
 * \brief Represent the game
 */
class Game
{
public:

    /**
     * \brief Constructor
     */
    explicit Game();

    /**
     * \brief Initialize the game
     * \param options A map of options
     *
     * This method initializes the game: it creates a window
     * and loads the configuration
     */
    void init(const std::map<Option, std::string>& options);

    /**
     * \brief Run the game
     *
     * This function runs the main loop of the game
     * it updates and display the game.
     */
    void run();

    /**
     * \brief Update the game
     *
     * This function updates the game.
     */
    void update();

    /**
     * \brief Display the game
     *
     * This function draws the game on the screen.
     */
    void render();

    /**
     * \brief Get the list of every entities on the map
     * \return Const reference to a vector containing all entities of the map
     *
     * If two entities are on the same cell, the last one is over the first one.
     */
    const std::vector<std::shared_ptr<Entity>>& getEntities() const;

    /**
     * \brief Get all entities put on a specific cell
     * \param position Position vector of the cell
     * \return A vector of pointers to entities put on the cell
     */
    std::vector<std::shared_ptr<Entity>> getEntitiesOnCell(sf::Vector2i position) const;

    /**
     * \brief Get all entities put on a specific cell
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return A vector of pointers to entities put on the cell
     */
    std::vector<std::shared_ptr<Entity>> getEntitiesOnCell(int x, int y) const;

    /**
     * \brief Get all entities in range of a specific cell
     * \param position Position vector of the cell
     * \param d Radius of the l<SUB>1</SUB> norm sphere that is searched
     * \return A vector of pointers to entities at distance less than are equal to d from the cell
     */
    std::vector<std::shared_ptr<Entity>> getEntitiesAroundCell(sf::Vector2i position, int d) const;

    /**
     * \brief Get all entities in range of a specific cell
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \param d Radius of the l<SUB>1</SUB> norm sphere that is searched
     * \return A vector of pointers to entities at distance less than are equal to d from the cell
     */
    std::vector<std::shared_ptr<Entity>> getEntitiesAroundCell(int x, int y, int d) const;

private:

    /**
     * \brief Update an entity
     * \return true if the entity could perform the action
     *
     * This function updates an entity.
     */
    bool update_entity(std::shared_ptr<Entity> entity, Action action);

    sf::RenderWindow window; ///< The render window of the game
    Configuration config; ///< The configuration of the game
    Renderer renderer; ///< The renderer

    std::shared_ptr<Menu> menu;

    Map* map; ///< The current map
    std::vector<std::shared_ptr<Entity>>* entities; ///< The current map entities
    std::size_t current_level; ///< The number of the curent level

    MapExploration map_exploration;

    std::vector<Level> dungeon; ///< The maps and entities of differents level of the dungeon

    EntityType entity_turn;
    float next_move;
    float move_time;
};
