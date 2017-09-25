/**
 * \file game.hpp
 * \brief Contains a class representing the game
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

private:

    sf::RenderWindow window; ///< The render window of the game
    Configuration config; ///< The configuration of the game
    Renderer renderer; ///< The renderer

    std::unique_ptr<Map> map; ///< The map
};
