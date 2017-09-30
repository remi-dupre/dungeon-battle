#include "generation.hpp"
#include <iostream>

std::pair<Map, std::vector<Entity>> generate(const GenerationMode &mode)
{
    // List of rooms.
    // A room is represented by its list of cells.
    std::vector<std::set<std::pair<int, int>>> rooms;

    // Create central element of rooms
    rooms.push_back(std::set<std::pair<int, int>>({
        std::make_pair((1./3.)*mode.width, (1./2.)*mode.height)
    }));
    rooms.push_back(std::set<std::pair<int, int>>({
        std::make_pair((2./3.)*mode.width, (1./2.)*mode.height)
    }));

    // For each room, add elements one by one
    for (int e = 1 ; e < mode.rooms_size ; e++)
    {
        for (size_t i_room = 0 ; i_room < rooms.size() ; i_room++)
        {
            // Process adjacent cells
            std::set<std::pair<int, int>> surrounding;
            // Function to add a cell to the surrounding if it is ok
            auto addCell = [mode, rooms](auto &surrounding, int x, int y)
            {
                if (x < 0 || y < 0)
                    return;
                if (x >= mode.width || y >= mode.height)
                    return;
                for (auto &room : rooms)
                {
                    if (room.count(std::make_pair(x, y)) > 0)
                    {
                        return;
                    }
                }
                surrounding.insert(std::make_pair(x, y));
            };
            for (auto &cell : rooms[i_room])
            {
                int x, y;
                std::tie(x, y) = cell;
                // addCell(surrounding, 0, 0);
                addCell(surrounding, x+1, y);
                addCell(surrounding, x, y+1);
                addCell(surrounding, x-1, y);
                addCell(surrounding, x, y-1);
            }
            if (surrounding.empty())
                continue;
            // Select a cell to insert
            std::set<std::pair<int, int>>::const_iterator selected(surrounding.begin());
            std::advance(selected, std::rand() % surrounding.size());
            rooms[i_room].insert(*selected);
        }
    }

    // Outputs result into the map
    Map map(mode.width, mode.height);
    for (auto &room : rooms)
    {
        for (auto &cell : room)
        {
            int x, y;
            std::tie(x, y) = cell;
            map.cellAt(x, y) = CellType::Floor;
        }
    }

    // Check wether a cell adjacent to (x, y) is a floor
    auto hasFloorNext = [mode, map](unsigned x, unsigned y)
    {
        if (x > 0 && map.cellAt(x-1, y) == CellType::Floor)
            return true;
        if (y > 0 && map.cellAt(x, y-1) == CellType::Floor)
            return true;
        if (x+1 < map.getWidth() && map.cellAt(x+1, y) == CellType::Floor)
            return true;
        if (y+1 < map.getHeight() && map.cellAt(x, y+1) == CellType::Floor)
            return true;
        return false;
    };
    // Add walls
    for (unsigned x = 0 ; x < map.getWidth() ; x++)
        for(unsigned y = 0 ; y < map.getHeight() ; y++)
            if (map.cellAt(x, y) == CellType::Empty && hasFloorNext(x, y))
                map.cellAt(x, y) = CellType::Wall;

    std::vector<Entity> entities;
    return std::make_pair(map, entities);
}
