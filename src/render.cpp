#include "render.hpp"


Renderer::Renderer() :
    tile_size(48.f)
{
    view.setCenter({10.f, 10.f});
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

            sf::Vertex v1({static_cast<float>(x)  , static_cast<float>(y)  });
            sf::Vertex v2({static_cast<float>(x)  , static_cast<float>(y+1)});
            sf::Vertex v3({static_cast<float>(x+1), static_cast<float>(y)  });
            sf::Vertex v4({static_cast<float>(x+1), static_cast<float>(y+1)});

            switch (cell)
            {
                // TODO(Krokmou): Assign a texture to the tiles
                case CellType::Floor: // Floor is grey
                    v1.color = {100, 100, 100};
                    v2.color = {100, 100, 100};
                    v3.color = {100, 100, 100};
                    v4.color = {100, 100, 100};
                    break;
                case CellType::Wall: // Walls are white
                    v1.color = {255, 255, 255};
                    v2.color = {255, 255, 255};
                    v3.color = {255, 255, 255};
                    v4.color = {255, 255, 255};
                    break;
                case CellType::Empty:
                    [[fallthrough]];
                default:
                    v1.color = {0, 0, 0};
                    v2.color = {0, 0, 0};
                    v3.color = {0, 0, 0};
                    v4.color = {0, 0, 0};
                    break;
            }

            map_vertices.push_back(v1);
            map_vertices.push_back(v2);
            map_vertices.push_back(v4);

            map_vertices.push_back(v1);
            map_vertices.push_back(v3);
            map_vertices.push_back(v4);
        }
    }
}

void Renderer::drawEntities(const std::vector<std::shared_ptr<Entity>>& entities)
{
    for (const auto& entity : entities)
    {

    }
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
    view.setViewport({x_offset/ 2.f, y_offset / 2.f, 1.f - x_offset, 1.f - y_offset});

    target.setView(view);

    target.draw(map_vertices.data(), map_vertices.size(), sf::PrimitiveType::Triangles);
}
