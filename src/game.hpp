/**
 * \file game.hpp
 * \brief Contains a class representing the game.
 */

#pragma once

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "map.hpp"
#include "render.hpp"


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
     * This function updates the game
     */
    void update();

    /**
     * \brief Display the game
     *
     * This function draws the game on the screen
     */
    void display();

    /**
     * \brief Get the list of every entities on the map.
     * \return Const reference to a vector containing all entities of the map.
     * If two entities are on the same cell, the last one is over the first one.
     */
    const std::vector<Entity>& getEntities() const;

    /**
     * \brief Get all entities putted on a specific cell
     * \param position Position vector of the cell
     * \return A vector of copies of the entities putted on the cell.
     */
    std::vector<Entity> getEntitiesOnCell(sf::Vector2i position) const;

    /**
     * \brief Get all entities putted on a specific cell
     * \param x X coordinate of the cell
     * \param y Y coordinate of the cell
     * \return A vector of copies of the entities putted on the cell.
     */
    std::vector<Entity> getEntitiesOnCell(int x, int y) const;


private:

    sf::RenderWindow window; ///< The render window of the game
    Configuration config; ///< The configuration of the game
    Renderer renderer; ///< The renderer

    Map map; ///< The map

    /**
     * \brief A vector containing every entities availables on the map
     */
    std::vector<Entity> entities;

};
