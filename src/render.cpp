#include "render.hpp"


Renderer::Renderer() :
    tile_size(32.f)
{
    tileset.loadFromFile("data/tileset.png");
    charlie_tex.loadFromFile("data/character01.png");
    entities_tileset.loadFromFile("data/entities.png");
}

void Renderer::drawMap(const Map& map)
{
    int map_w = map.getWidth();
    int map_h = map.getHeight();

    map_vertices.clear();
    map_vertices.reserve(map_w * map_h * 4);

    for (int x = 0; x < map_w; x++)
    {
        for (int y = 0; y < map_h; y++)
        {
            CellType cell = map.cellAt(x, y);

            drawCell({x, y}, cell, map);
        }
    }
}

#include <iostream>

void Renderer::drawEntities(const std::vector<std::shared_ptr<Entity>>& entities, float frame_time)
{
    entities_vertices.clear();
    entities_vertices.push_back({0, {}});
    entities_vertices[0].second.reserve(entities.size() * 4);

    charlie.clear();

    for (const auto& entity : entities)
    {
        sf::Vector2f p = tile_size * static_cast<sf::Vector2f>(entity->getPosition());
        sf::Vector2f old_p = tile_size * static_cast<sf::Vector2f>(entity->getOldPosition());

        float frame = 0.f;

        // Interpolate the current position with the old one if the entity is moving
        if (entity->isMoving())
            p += frame_time * (old_p - p);
        // Set the correct frame is the entity is doing something
        if (entity->isMoving() || entity->isAttacking())
            frame = std::floor(frame_time * 3.999f);

        sf::Vertex v1, v2, v3, v4;

        v1.position = {p.x, p.y};
        v2.position = {p.x, p.y + tile_size};
        v3.position = {p.x + tile_size, p.y};
        v4.position = {p.x + tile_size, p.y + tile_size};

        if (entity->getType() == EntityType::Hero)
        {
            v1.position = {p.x - 1.f, p.y - 25.f};
            v2.position = {p.x - 1.f, p.y + tile_size - 8.f};
            v3.position = {p.x + tile_size - 1.f, p.y - 25.f};
            v4.position = {p.x + tile_size - 1.f, p.y + tile_size - 8.f};

            switch (entity->getOrientation())
            {
            case Direction::Down:
                v1.texCoords = {frame * 32.f, 0.f};
                v2.texCoords = {frame * 32.f, 48.f};
                v3.texCoords = {frame * 32.f + 31.f, 0.f};
                v4.texCoords = {frame * 32.f + 31.f, 48.f};
                break;

            case Direction::Left:
                v1.texCoords = {frame * 32.f, 49.f};
                v2.texCoords = {frame * 32.f, 96.f};
                v3.texCoords = {frame * 32.f + 31.f, 49.f};
                v4.texCoords = {frame * 32.f + 31.f, 96.f};
                break;

            case Direction::Right:
                v1.texCoords = {frame * 32.f, 97.f};
                v2.texCoords = {frame * 32.f, 144.f};
                v3.texCoords = {frame * 32.f + 31.f, 97.f};
                v4.texCoords = {frame * 32.f + 31.f, 144.f};
                break;

            case Direction::Up:
                v1.texCoords = {frame * 32.f, 145.f};
                v2.texCoords = {frame * 32.f, 192.f};
                v3.texCoords = {frame * 32.f + 31.f, 145.f};
                v4.texCoords = {frame * 32.f + 31.f, 192.f};
                break;

            case Direction::None:
            default:
                break;
            }

            charlie.push_back(v1);
            charlie.push_back(v2);
            charlie.push_back(v4);

            charlie.push_back(v1);
            charlie.push_back(v3);
            charlie.push_back(v4);

            continue;
        }

        sf::Color slime_color = {static_cast<sf::Uint8>(Random::uniform_int(0, 255)),
                                 static_cast<sf::Uint8>(Random::uniform_int(0, 255)),
                                 static_cast<sf::Uint8>(Random::uniform_int(0, 255))};

        // if (!entity->isMoving())
        //     frame = 0.f;

        switch (entity->getType())
        {
            case EntityType::Hero:
                break;

            case EntityType::Stairs:
                v1.texCoords = {64.f, 0.f};
                v2.texCoords = {64.f, 31.f};
                v3.texCoords = {95.f, 0.f};
                v4.texCoords = {95.f, 31.f};
               break;

            case EntityType::Monster:
                v1.texCoords = {32.f, frame * 32.f};
                v2.texCoords = {32.f, frame * 32.f + 31.f};
                v3.texCoords = {63.f, frame * 32.f};
                v4.texCoords = {63.f, frame * 32.f + 31.f};

                v1.color = slime_color;
                v2.color = slime_color;
                v3.color = slime_color;
                v4.color = slime_color;
                break;

            case EntityType::None:
                [[fallthrough]];
            default:
                break;
        }

        entities_vertices[0].second.push_back(v1);
        entities_vertices[0].second.push_back(v2);
        entities_vertices[0].second.push_back(v4);

        entities_vertices[0].second.push_back(v1);
        entities_vertices[0].second.push_back(v3);
        entities_vertices[0].second.push_back(v4);
    }
}

void Renderer::setViewCenter(sf::Vector2f center)
{
    view.setCenter(tile_size * center);
}

void Renderer::display(sf::RenderTarget& target)
{
    // Keep aspect ratio
    float screen_ratio = static_cast<float>(Configuration::default_configuration.height) /
        static_cast<float>(Configuration::default_configuration.width);

    float screen_x = static_cast<float>(target.getSize().x);
    float screen_y = static_cast<float>(target.getSize().y);

    float ratio = screen_y / screen_x;

    float x_offset = 0.f;
    float y_offset = 0.f;

    if (ratio > screen_ratio)
    {
        float y = screen_x * screen_ratio;
        y_offset = (screen_y - y) / screen_y;
    }
    else if (ratio < screen_ratio)
    {
        float x =  screen_y / screen_ratio;
        x_offset = (screen_x - x) / screen_x;
    }

    view.setSize({static_cast<float>(Configuration::default_configuration.width),
                static_cast<float>(Configuration::default_configuration.height)});
    view.setViewport({x_offset / 2.f, y_offset / 2.f, 1.f - x_offset, 1.f - y_offset});

    target.setView(view);

    sf::RenderStates map_rstates(&tileset);
    target.draw(map_vertices.data(),
                map_vertices.size(),
                sf::PrimitiveType::Triangles,
                map_rstates);


    sf::RenderStates entities_rstates(&entities_tileset);
    for (auto& entity_layer : entities_vertices)
    {
        target.draw(entity_layer.second.data(),
                    entity_layer.second.size(),
                    sf::PrimitiveType::Triangles,
                    entities_rstates);
    }

    sf::RenderStates charlie_rstates(&charlie_tex);
    target.draw(charlie.data(),
                charlie.size(),
                sf::PrimitiveType::Triangles,
                charlie_rstates);
}

void Renderer::drawCell(sf::Vector2i coords, CellType cell, const Map& map)
{
    if (cell == CellType::Empty)
        return;

    Random::seed(std::hash<sf::Vector2i>{}(coords));

    sf::Vector2f p = tile_size * static_cast<sf::Vector2f>(coords);

    sf::Vertex v1, v2, v3, v4;

    v1.position = p;
    v2.position = {p.x, p.y + tile_size};
    v3.position = {p.x + tile_size, p.y};
    v4.position = {p.x + tile_size, p.y + tile_size};

    // Texture rectangles for floor are hardcoded for now
    const sf::Vector2f tiles_coord[] = {
        {Random::uniform_int(0, 5) * 32.f, 128.f},
        {0.f, 96.f}, {32.f, 96.f}, {0.f, 64.f}, {64.f, 96.f},
        {32.f, 64.f}, {64.f, 64.f}, {0.f, 32.f}, {96.f, 96.f},
        {96.f, 64.f}, {128.f, 64.f}, {32.f, 32.f}, {160.f, 64.f},
        {64.f, 32.f}, {96.f, 32.f},
        {Random::uniform_int(0, 5) * 32.f, 0.f}
    };

    sf::Vector2i next_tiles[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    int tile_index = 0;

    for (int i = 0; i < 4; i++)
    {
        if (map.wallNext(coords + next_tiles[i]))
            tile_index |= 1 << i;
    }
    if (map.wallNext(coords))
        tile_index = 15;

    assert(0 <= tile_index && tile_index < 16);

    int tile_nb;

    sf::Vector2f t = tiles_coord[tile_index];

    switch (cell)
    {
        case CellType::Floor:
            v1.texCoords = t;
            v2.texCoords = {t.x, t.y + 31.f};
            v3.texCoords = {t.x + 31.f, t.y};
            v4.texCoords = {t.x + 31.f, t.y + 31.f};
            break;

        case CellType::Wall:
            tile_nb = Random::uniform_int(0, 3);
            v1.texCoords = {32.f * tile_nb, 160.f};
            v2.texCoords = {32.f * tile_nb, 191.f};
            v3.texCoords = {32.f * tile_nb + 31.f, 160.f};
            v4.texCoords = {32.f * tile_nb + 31.f, 191.f};
            break;

        case CellType::Empty:
            [[fallthrough]];
        default:
            return; // Don't draw empty/unkonwn cells
    }

    map_vertices.push_back(v1);
    map_vertices.push_back(v2);
    map_vertices.push_back(v4);

    map_vertices.push_back(v1);
    map_vertices.push_back(v3);
    map_vertices.push_back(v4);
}
