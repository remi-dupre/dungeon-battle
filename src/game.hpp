/**
 * \file game.hpp
 * \brief Contains a class representing the game
 */

#include <SFML/Graphics.hpp>

#include "config.hpp"
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
    Game();

    /**
     * \brief Initialize the game
     *
     * This method initializes the game: it creates a window
     * and loads the configuration
     */
    void init();

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

    sf::RenderWindow window;
    Configuration config;
    Renderer renderer;
};
