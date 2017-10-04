/**
 * \file render.hpp
 * \brief Manage the rendering of the game.
 */

#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "map.hpp"
#include "rand.hpp"


/**
 * \brief Manage the rendering of the game
 *
 * This class is used the render the game on the window.
 */
class Renderer
{
public:

    /**
     * \brief Construct a renderer
     */
    explicit Renderer();

    /**
     * \brief Delete the copy constructor
     */
    Renderer(const Renderer&) = delete;

    /**
     * \brief Draw the map
     * \param map The map to draw
     *
     * This function draws the map.
     */
    void drawMap(const Map& map);

    /**
     * \brief Draw the entities
     * \param entities A vector of entities to draw
     *
     * This function draws the entities
     */
    void drawEntities(const std::vector<std::shared_ptr<Entity>>& entities);

    /**
     * \brief Set the view center
     */
    void setViewCenter(sf::Vector2u center);

    /**
     * \brief Display on the window the objects drawn before
     * \param target The RenderTarget to draw on
     *
     * This function display the objects drawn before
     * on the given RenderTarget.
     */
    void display(sf::RenderTarget& target);

private:

    /**
     * \brief Add a single cell to the vertex array
     *
     *
     */
    void drawCell(sf::Vector2i coords, CellType cell);

    const float tile_size; ///< Size of the tiles on screen in pixels
    std::vector<sf::Vertex> map_vertices; ///< Vertex array used to render the map
    sf::Texture tileset; ///< Texture of the tiles

    std::vector<std::pair<int, std::vector<sf::Vertex>>> entities_vertices; ///< Vertex array used to render entities

    sf::View view;
};
