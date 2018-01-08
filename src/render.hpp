/**
 * \file render.hpp
 * \brief Manage the rendering of the game.
 */

#pragma once

#include <cassert>
#include <memory>

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "entity.hpp"
#include "exploration.hpp"
#include "map.hpp"
#include "math.hpp"
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
     * \brief Draw the current game state
     * \param map The map
     * \param map_exploration The current state of exploration of the map
     * \param entities The vector of entities
     * \param frame_progress The current frame progress
     * \param center_entity The entity to center the view on
     */
    void drawGame(const Map& map,
                  MapExploration& map_exploration,
                  const std::vector<std::shared_ptr<Entity>>& entities,
                  std::shared_ptr<Entity> center_entity,
                  float frame_progress,
                  const Configuration& config);

    /**
     * \brief Display on the window the objects drawn before
     * \param target The RenderTarget to draw on
     *
     * This function display the objects drawn before
     * on the given RenderTarget.
     */
    void display(sf::RenderTarget& target, float frame_progress);

private:

    /**
     * \brief Draw an entity
     * \param entity The entity to draw
     * \param cell_explored Whether the entity is on explored cell or not
     * \param frame_progress The current frame_progress
     */
    void drawEntity(std::shared_ptr<Entity> entity, bool cell_explored, float frame_progress);

    /**
     * \brief Draw a single cell
     * \param coords The coordinates of the cell
     * \param cell The type of the cell
     *
     * This function adds the vertices of a cell to
     * the vertex array to draw it later
     */
    void drawCell(sf::Vector2i coords, CellType cell, const Map& map, MapExploration& map_exploration, const Configuration& config);

    const float tile_size = 32.f; ///< Size of the tiles on screen in pixels

    unsigned int seed; ///< Seed used for rendering tiles

    float time_since_last_frame = 0.f;

    sf::View view; ///< The current view of the rendering
    std::shared_ptr<Entity> entity_center_view;

    sf::Vector2i world_view_size;

    std::vector<sf::Vertex> map_vertices_bg; ///< Vertex array used to render the background
    std::vector<sf::Vertex> map_vertices_fg; ///< Vertex array used to render the foreground

    std::vector<sf::Sprite> entities_sprites;

    sf::Text hero_life; ///< Display the life of the hero
    sf::Text hero_xp;   ///< Display the XP of the hero
};
