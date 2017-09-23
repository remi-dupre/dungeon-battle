/**
 * \file render.hpp
 * \brief Manage the rendering of the game
 */

#pragma once

#include <SFML/Graphics.hpp>

#include "map.hpp"


/**
 * \brief Manage the rendering of the game
 *
 * This class is used the render the game on the window
 */
class Renderer
{
public:

    /**
     * \brief Construct a renderer
     */
    Renderer();

    /**
     * \brief Delete the copy constructor
     */
    Renderer(const Renderer&) = delete;

    /**
     * \brief Draw the map
     */
    void drawMap(const Map& map);

    /**
     * \brief Display on the window the objects drawn before
     */
    void display(sf::RenderWindow& window);

private:

    const float tile_size; ///< Size of the tiles on screen in pixels

    std::vector<sf::Vertex> map_vertices; ///< Vertex array used to render the map
    sf::Texture map_texture; ///< Texture of the tiles
};
