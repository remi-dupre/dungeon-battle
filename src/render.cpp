#include "render.hpp"


Renderer::Renderer() :
    tile_size(40.f)
{
}


void Renderer::drawMap(const Map& map)
{
    int map_w = map.getWidth();
    int map_h = map.getHeight();

    map_vertices.reserve(map_w * map_h * 4);

    for (int x = 0; x < map_w; x++)
    {
        for (int y = 0; y < map_h; y++)
        {
            CellType cell = map.cellAt(x, y);

            sf::Vertex v1({ x    * tile_size,  y    * tile_size});
            sf::Vertex v2({ x    * tile_size, (y+1) * tile_size});
            sf::Vertex v3({(x+1) * tile_size,  y    * tile_size});
            sf::Vertex v4({(x+1) * tile_size, (y+1) * tile_size});

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
                case CellType::Empty: [[fallthrough]]
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
    }
}

void Renderer::display(sf::RenderTarget& target)
{
    target.draw(map_vertices.data(), map_vertices.size(), sf::PrimitiveType::Triangles);
}
