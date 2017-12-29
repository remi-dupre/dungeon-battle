/**
 * \file render.hpp
 * \brief Manage the rendering of the game.
 */

#pragma once

#include <cassert>
#include <memory>

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "map.hpp"
#include "math.hpp"
#include "menu.hpp"
#include "rand.hpp"
#include "utility.hpp"


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
     * \brief Set the view center
     */
    void setViewCenter(sf::Vector2f center);

    /**
     * \brief Draw the map
     * \param map The map to draw
     * \warning In order to draw correctly the map,
     * this function MUST be called AFTER `setViewCenter`
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
    void drawEntities(const std::vector<std::shared_ptr<Entity>>& entities, float frame_time);

    /**
     * \brief Draw a menu
     * \param menu The menu to draw
     *
     * This function draws a menu
     */
    void drawMenu(std::shared_ptr<const Menu> menu);

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
     * \brief Draw a single cell
     * \param coords The coordinates of the cell
     * \param cell The type of the cell
     *
     * This function adds the vertices of a cell to the vertex array
     * to draw it later
     */
    void drawCell(sf::Vector2i coords, CellType cell, const Map& map);

    unsigned int seed; ///< Seed used for rendering tiles

    sf::View view; ///< The current view of the rendering

    static constexpr float tile_size = 32.f; ///< Size of the tiles on screen in pixels

    std::vector<sf::Vertex> map_vertices; ///< Vertex array used to render the map
    //sf::Texture tileset; ///< Texture of the tiles

    std::vector<sf::Sprite> entities_sprites;
    //std::map<Class, sf::Texture> entities_textures;

    // std::vector<sf::Vertex> entities_vertices; ///< Vertex array used to render entities
    // sf::Texture entities_tileset; ///< Texture of the characters
    // std::vector<sf::Vertex> charlie;
    // sf::Texture charlie_tex; ///< Texture of the characters

    //sf::Font font;      ///< Font used to render texts in the game
    sf::Text hero_life; ///< Display the life of the hero
    sf::Text hero_xp;   ///< Display the XP of the hero

    std::vector<sf::Text> menu_texts;
};
