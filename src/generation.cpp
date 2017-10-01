#include "generation.hpp"


int pattern_min_x(const Pattern& pattern)
{
    int min_x = std::numeric_limits<int>::max();
    for (auto cell : pattern)
        min_x = std::min(min_x, cell.first);
    return min_x;
}

int pattern_max_x(const Pattern& pattern)
{
    int max_x = std::numeric_limits<int>::min();
    for (auto cell : pattern)
        max_x = std::max(max_x, cell.first);
    return max_x;
}

int pattern_min_y(const Pattern& pattern)
{
    int min_y = std::numeric_limits<int>::max();
    for (auto cell : pattern)
        min_y = std::min(min_y, cell.second);
    return min_y;
}

int pattern_max_y(const Pattern& pattern)
{
    int max_y = std::numeric_limits<int>::min();
    for (auto cell : pattern)
        max_y = std::max(max_y, cell.first);
    return max_y;
}


Pattern normalized_pattern(Pattern& pattern)
{
    int min_x = pattern_min_x(pattern);
    int min_y = pattern_min_y(pattern);

    Pattern normalized;
    for (auto& cell : pattern)
        normalized.insert({cell.first-min_x, cell.second-min_y});
    return normalized;
}

Pattern merged_patterns(
    const std::vector<std::pair<int, int>>& positions,
    const std::vector<Pattern>& patterns)
{
    std::set<std::pair<int, int>> fullMap;
    for (size_t i_pattern = 0 ; i_pattern < patterns.size() ; i_pattern++)
    {
        int x_center, y_center;
        std::tie(x_center, y_center) = positions[i_pattern];
        for(auto pattern_cell : patterns[i_pattern])
        {
            int x_cell, y_cell;
            std::tie(x_cell, y_cell) = pattern_cell;
            fullMap.insert({x_cell + x_center, y_cell + y_center});
        }
    }
    return fullMap;
}

Map map_of_pattern(const Pattern& pattern, int width, int height)
{
    Map map(width, height);

    // Add cells of the pattern on the floor
    for (auto &cell : pattern)
    {
        int x, y;
        std::tie(x, y) = cell;
        if (x > 0  && x < width && y > 0 && y < height)
            map.cellAt(x, y) = CellType::Floor;
    }

    // Function to check wether a cell adjacent to (x, y) is a floor
    auto hasFloorNext = [height, width, map](int x, int y)
    {
        if (x > 0 && map.cellAt(x-1, y) == CellType::Floor)
            return true;
        if (y > 0 && map.cellAt(x, y-1) == CellType::Floor)
            return true;
        if (x+1 < width && map.cellAt(x+1, y) == CellType::Floor)
            return true;
        if (y+1 < height && map.cellAt(x, y+1) == CellType::Floor)
            return true;
        return false;
    };
    // Add walls
    for (int x = 0 ; x < width ; x++)
        for(int y = 0 ; y < height ; y++)
            if (map.cellAt(x, y) == CellType::Empty && hasFloorNext(x, y))
                map.cellAt(x, y) = CellType::Wall;

    return map;
}


Pattern make_hallway(std::pair<int, int> cell1, std::pair<int, int> cell2)
{
    int x1, y1, x2, y2;
    std::tie(x1, y1) = cell1;
    std::tie(x2, y2) = cell2;

    int x=0, y=0;
    Pattern path;
    path.insert({0, 0});
    while (x1+x != x2 || y1+y != y2)
    {
        if (x1+x < x2)
            x++;
        if (x1+x > x2)
            x--;
        if (y1+y < y2)
            y++;
        if (y1+y > y2)
            y--;
        path.insert({x, y});
        path.insert({x-1, y});
        path.insert({x, y-1});
        path.insert({x+1, y});
        path.insert({x, y+1});
    }

    return path;
}

Pattern generate_cave(int size)
{
    if (size < 0)
        size = 1;

    Pattern cells;
    Pattern surrounding;
    // Add cells around the first one
    surrounding.insert(std::make_pair(0, 0));
    // Add a new cell to surrounding, if it isn't already in the patern.
    auto addSurrounding = [&cells, &surrounding](int x, int y) -> void
    {
        if (cells.count(std::make_pair(x, y)) == 0)
            surrounding.insert(std::make_pair(x, y));
    };

    for(int nb_cells = 0 ; nb_cells < size ; nb_cells++)
    {
        // Select a cell to insert
        auto selected = surrounding.begin();
        std::advance(selected, Random::uniform_int(0, surrounding.size()-1));
        cells.insert(*selected);
        // Refresh surrounding set of the pattern
        int new_x, new_y;
        std::tie(new_x, new_y) = *selected;
        addSurrounding(new_x+1, new_y);
        addSurrounding(new_x-1, new_y);
        addSurrounding(new_x, new_y+1);
        addSurrounding(new_x, new_y-1);
        // Can't select this cell anymore
        surrounding.erase(selected);
    }

    return cells;
}

std::pair<Map, std::vector<Entity>> generate(const GenerationMode &mode)
{
    // List of rooms.
    std::vector<Pattern> patterns;
    std::vector<std::pair<int, int>> positions;

    // Create rooms of random size
    for (int i_room = 0 ; i_room < mode.nb_rooms ; i_room++)
    {
        int room_size = Random::uniform_int(mode.room_min_size, mode.room_max_size);
        patterns.push_back(generate_cave(room_size));
    }

    // Position rooms
    int map_right = 0; // right position of the end of last room
    for (int i_room = 0 ; i_room < mode.nb_rooms ; i_room++)
    {
        int room_left = pattern_min_x(patterns[i_room]);
        int room_right = pattern_max_x(patterns[i_room]);
        int pos_x = 10 + map_right - room_left;

        map_right += 10 + (room_right - room_left);
        positions.push_back({pos_x, Random::uniform_int(-50, 50)});
    }

    // Add ways between rooms
    for (int i_room = 1 ; i_room < mode.nb_rooms ; i_room++)
    {
        patterns.push_back(make_hallway(positions[i_room-1], positions[i_room]));
        positions.push_back(positions[i_room-1]);
    }


    Pattern cells = merged_patterns(positions, patterns);

    // Outputs result into the map
    Map map = map_of_pattern(normalized_pattern(cells), pattern_max_x(cells), pattern_max_y(cells));

    std::vector<Entity> entities;
    return {map, entities};
}
