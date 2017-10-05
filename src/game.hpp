/**
 * \file game.hpp
 * \brief Contains a class representing the game.
 */

#pragma once

#include <map>
#include <memory>
#include <random>
#include <string>

#include <SFML/Graphics.hpp>

#include "config.hpp"

#include "args.hpp"
#include "control.hpp"
#include "generation.hpp"
#include "map.hpp"
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
    void display();

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
    std::vector<std::shared_ptr<Entity>> getEntitiesOnCell(sf::Vector2u position) const;

    /**
     * \brief Get all entities put on a specific cell
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return A vector of pointers to entities put on the cell
     */
    std::vector<std::shared_ptr<Entity>> getEntitiesOnCell(unsigned int x, unsigned int y) const;

    /**
     * \brief Get all entities in range of a specific cell
     * \param position Position vector of the cell
     * \param d Radius of the l<SUB>1</SUB> norm sphere that is searched
     * \return A vector of pointers to entities at distance less than are equal to d from the cell
     */
    std::vector<std::shared_ptr<Entity>> getEntitiesAroundCell(sf::Vector2u position, unsigned int d) const;

    /**
     * \brief Get all entities in range of a specific cell
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \param d Radius of the l<SUB>1</SUB> norm sphere that is searched
     * \return A vector of pointers to entities at distance less than are equal to d from the cell
     */
    std::vector<std::shared_ptr<Entity>> getEntitiesAroundCell(unsigned int x, unsigned int y, unsigned int d) const;

private:

    sf::RenderWindow window; ///< The render window of the game
    Configuration config; ///< The configuration of the game
    Renderer renderer; ///< The renderer

    Map map; ///< The map

    std::vector<std::shared_ptr<Entity>> entities; ///< The entities
};
