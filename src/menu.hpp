/**
 * \file menu.hpp
 * \brief Contains a class representing the menu.
 */

#pragma once

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "render.hpp"


/**
 * \brief Reprensent the menu.
*/
class Menu
{
public:

    /**
     * \brief Menu default constructor.
     */
     explicit Menu(const Configuration& config);

    /**
     * \brief Looping while we are displaying the menu.
     *
     * While no decision has been taken to leave the menu.
     */
    void run();

private:
    int current_line;
    int chosen_character;

    Configuration config;
    sf::RenderWindow window;
    Renderer renderer;

};
