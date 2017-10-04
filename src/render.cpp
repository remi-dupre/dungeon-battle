#include "render.hpp"


Renderer::Renderer() :
    tile_size(32.f)
{
    tileset.loadFromFile("tileset.png");
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

            drawCell({x, y}, cell);
        }
    }
}

void Renderer::drawEntities(const std::vector<std::shared_ptr<Entity>>& entities)
{
    entities_vertices.clear();
    entities_vertices.push_back({0, {}});
    entities_vertices[0].second.reserve(entities.size() * 4);

    for (const auto& entity : entities)
    {
        int x = entity->getPosition().x;
        int y = entity->getPosition().y;

        sf::Vertex v1({static_cast<float>(x) + 0.1f  , static_cast<float>(y)   + 0.1f});
        sf::Vertex v2({static_cast<float>(x) + 0.1f  , static_cast<float>(y+1) - 0.1f});
        sf::Vertex v3({static_cast<float>(x+1) - 0.1f, static_cast<float>(y)   + 0.1f});
        sf::Vertex v4({static_cast<float>(x+1) - 0.1f, static_cast<float>(y+1) - 0.1f});

        switch (entity->getType())
        {
            case EntityType::Hero:
                v1.color = sf::Color::Red;
                v2.color = sf::Color::Red;
                v3.color = sf::Color::Red;
                v4.color = sf::Color::Red;
                break;
            case EntityType::Stairs:
                v1.color = sf::Color::Yellow;
                v2.color = sf::Color::Yellow;
                v3.color = sf::Color::Yellow;
                v4.color = sf::Color::Yellow;
                break;
            case EntityType::Monster:
                v1.color = sf::Color::Green;
                v2.color = sf::Color::Green;
                v3.color = sf::Color::Green;
                v4.color = sf::Color::Green;
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

void Renderer::setViewCenter(sf::Vector2u center)
{
    view.setCenter(static_cast<sf::Vector2f>(center));
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

    view.setSize({Configuration::default_configuration.width / tile_size,
                  Configuration::default_configuration.height / tile_size});
    view.setViewport({x_offset / 2.f, y_offset / 2.f, 1.f - x_offset, 1.f - y_offset});

    target.setView(view);

    sf::RenderStates rstates(&tileset);
    target.draw(map_vertices.data(), map_vertices.size(), sf::PrimitiveType::Triangles, rstates);

    for (auto& entity_layer : entities_vertices)
    {
        target.draw(entity_layer.second.data(), entity_layer.second.size(), sf::PrimitiveType::Triangles);
    }
}

void Renderer::drawCell(sf::Vector2i coords, CellType cell)
{
    if (cell == CellType::Empty)
        return;

    Random::seed(17 * coords.x + 53 * coords.y);

    sf::Vector2f c = static_cast<sf::Vector2f>(coords);

    sf::Vertex v1({c});
    sf::Vertex v2({c.x, c.y + 1.f});
    sf::Vertex v3({c.x + 1.f, c.y});
    sf::Vertex v4({c.x + 1.f, c.y + 1.f});

    int tile_nb;

    switch (cell)
    {
    case CellType::Floor:
        tile_nb = Random::uniform_int(0, 5);
        v1.texCoords = {32.f*tile_nb, 32.f};
        v2.texCoords = {32.f*tile_nb, 63.f};
        v3.texCoords = {32.f*tile_nb + 31.f, 32.f};
        v4.texCoords = {32.f*tile_nb + 31.f, 63.f};
        v1.color = {75, 75, 75};
        v2.color = {75, 75, 75};
        v3.color = {75, 75, 75};
        v4.color = {75, 75, 75};
        break;
    case CellType::Wall:
        tile_nb = Random::uniform_int(0, 3);
        v1.texCoords = {32.f*tile_nb, 0.f};
        v2.texCoords = {32.f*tile_nb, 31.f};
        v3.texCoords = {32.f*tile_nb + 31.f, 0.f};
        v4.texCoords = {32.f*tile_nb + 31.f, 31.f};
        break;
    case CellType::Empty:
        [[fallthrough]];
    default:
        break;
    }

    map_vertices.push_back(v1);
    map_vertices.push_back(v2);
    map_vertices.push_back(v4);

    map_vertices.push_back(v1);
    map_vertices.push_back(v3);
    map_vertices.push_back(v4);
}
