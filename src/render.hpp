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
     * \param map The map to draw
     *
     * This function set the internal data of the Renderer class
     * to draw the map on the window later.
     */
    void drawMap(const Map& map);

    /**
     * \brief Display on the window the objects drawn before
     * \param target The RenderTarget to draw on
     *
     * This function display the objects drawn before
     * on the given RenderTarget.
     */
    void display(sf::RenderTarget& target);

private:

    const float tile_size; ///< Size of the tiles on screen in pixels

    std::vector<sf::Vertex> map_vertices; ///< Vertex array used to render the map
    sf::Texture map_texture; ///< Texture of the tiles
};
