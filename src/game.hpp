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

#include "generation/generator.hpp"

#include "args.hpp"
#include "config.hpp"
#include "control.hpp"
#include "exploration.hpp"
#include "map.hpp"
#include "menu/menu.hpp"
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

    /**
     * \brief Create a new game
     * \param save_path The name of the game
     * \param hero_class The class of the hero
     */
    void newGame(const std::string& save_path, Class hero_class);
    /**
     * \brief Load a game
     * \param save_path The name of the game to load
     */
    bool loadGame(const std::string& save_path);
    /**
     * \brief Save the current game
     */
    bool saveGame();

    /**
     * \brief Generate enough map arround the player.
     */
    void loadArround();

    Configuration config; ///< The configuration of the game
    float move_time; ///< The length of the animations

    sf::RenderWindow window; ///< The render window of the game
    Renderer renderer; ///< The renderer

    std::shared_ptr<Menu> menu; ///< The current menu

    std::string game_name; ///< The current game name (where to save it)

    Generator *generator; ///< Engine generating the map

    Map* map; ///< The current map
    std::vector<std::shared_ptr<Entity>>* entities; ///< The current map entities
    MapExploration *map_exploration; ///< The exploration state of the map

    std::size_t current_level; ///< The number of the curent level
    std::vector<Level> dungeon; ///< The maps and entities of differents level of the dungeon
    std::vector<MapExploration> exploration;

    EntityType entity_turn; ///< Tell whether it is the player or the monsters to play
    float next_move; ///< Time until animation terminates
};
