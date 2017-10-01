#include "generation.hpp"
#include "rand.hpp"


Pattern generateCave(int size)
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
        std::advance(selected, std::rand() % surrounding.size());
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

int getPatternMinX(const Pattern& pattern)
{
    int min_x = std::numeric_limits<int>::max();
    for (auto cell : pattern)
        min_x = std::min(min_x, cell.first);
    return min_x;
}

int getPatternMaxX(const Pattern& pattern)
{
    int max_x = std::numeric_limits<int>::min();
    for (auto cell : pattern)
        max_x = std::max(max_x, cell.first);
    return max_x;
}

int getPatternMinY(const Pattern& pattern)
{
    int min_y = std::numeric_limits<int>::max();
    for (auto cell : pattern)
        min_y = std::min(min_y, cell.second);
    return min_y;
}

int getPatternMaxY(const Pattern& pattern)
{
    int max_y = std::numeric_limits<int>::min();
    for (auto cell : pattern)
        max_y = std::max(max_y, cell.first);
    return max_y;
}


Pattern normalizedPattern(Pattern& pattern)
{
    int min_x = getPatternMinX(pattern);
    int min_y = getPatternMinY(pattern);

    Pattern normalized;
    for (auto& cell : pattern)
        normalized.insert({cell.first-min_x, cell.second-min_y});
    return normalized;
}

Pattern mergePatterns(
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


Map mapOfPattern(const Pattern& pattern, int width, int height)
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


std::pair<Map, std::vector<Entity>> generate(const GenerationMode &mode)
{
    // List of rooms.
    std::vector<Pattern> rooms;
    std::vector<std::pair<int, int>> room_positions;

    // Create rooms of random size
    for (int i_room = 0 ; i_room < mode.nb_rooms ; i_room++)
        rooms.push_back(generateCave(Random::uniform_int(mode.room_min_size, mode.room_max_size)));

    int map_right = 0; // right position of the end of last room
    for (auto& room : rooms)
    {
        int room_left = getPatternMinX(room);
        int room_right = getPatternMaxX(room);
        int pos_x = 2 + map_right - room_left;

        map_right += 2 + (room_right - room_left);
        room_positions.push_back({pos_x, 0});
    }

    Pattern cells = mergePatterns(room_positions, rooms);

    // Outputs result into the map
    Map map = mapOfPattern(normalizedPattern(cells), getPatternMaxX(cells), getPatternMaxY(cells));

    std::vector<Entity> entities;
    return {map, entities};
}
