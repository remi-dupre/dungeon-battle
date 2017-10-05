#include "generation.hpp"
#include "rand.hpp"


int pattern_min_x(const Pattern& pattern)
{
    return std::min_element(
        begin(pattern), end(pattern),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        }
    )->first;
}

int pattern_max_x(const Pattern& pattern)
{
    return std::max_element(
        begin(pattern), end(pattern),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        }
    )->first;
}

int pattern_min_y(const Pattern& pattern)
{
    return std::min_element(
        begin(pattern), end(pattern),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }
    )->second;
}

int pattern_max_y(const Pattern& pattern)
{
    return std::max_element(
        begin(pattern), end(pattern),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }
    )->second;
}


Pattern normalized_pattern(Pattern& pattern, std::vector<std::shared_ptr<Entity>>& entities)
{
    int min_x = pattern_min_x(pattern);
    int min_y = pattern_min_y(pattern);

    Pattern normalized;

    for (auto& cell : pattern)
    {
        assert(cell.first  - min_x >= 0);
        assert(cell.second - min_y >= 0);
        normalized.insert({cell.first - min_x, cell.second - min_y});
    }

    for (auto& entity : entities)
    {
        auto pos = entity->getPosition();
        entity->setPosition({pos.x - min_x, pos.y - min_y});
    }

    return normalized;
}

Pattern merged_patterns(
    const std::vector<std::pair<int, int>>& positions,
    const std::vector<Pattern>& patterns)
{
    // Every pattern must have a position
    assert(positions.size() == patterns.size());

    std::set<std::pair<int, int>> fullMap;

    for (size_t i_pattern = 0 ; i_pattern < patterns.size() ; i_pattern++)
    {
        int x_center = positions[i_pattern].first;
        int y_center = positions[i_pattern].second;

        for(auto pattern_cell : patterns[i_pattern])
        {
            int x_cell = pattern_cell.first;
            int y_cell = pattern_cell.second;

            fullMap.insert({x_cell + x_center, y_cell + y_center});
        }
    }

    return fullMap;
}

Map map_of_pattern(const Pattern& pattern)
{
    // Keeps a 1-unit margin at every border
    int width = pattern_max_x(pattern) + 3;
    int height = pattern_max_y(pattern) + 3;
    Map map(width, height);

    // Add cells of the pattern on the floor
    for (auto &cell : pattern)
    {
        int x = cell.first;
        int y = cell.second;

        assert(x+1 > 0 && x+1 < width);
        assert(y+1 > 0 && y+1 < height);
        map.cellAt(x+1, y+1) = CellType::Floor;
    }

    // Function to check wether a cell adjacent to (x, y) is a floor
    auto hasFloorNext = [height, width, map](int x, int y)
    {
        for (int i = x-1 ; i <= x+1 ; i++)
        {
            for (int j = y-1 ; j <= y+1 ; j++)
            {

                if (i >= 0 && i < width && j >= 0 && j < height)
                {
                    if ((i != x || j != y) && map.cellAt(i, j) == CellType::Floor)
                        return true;
                }
            }
        }

        return false;
    };

    // Add walls
    for (int x = 0 ; x < width ; x++)
        for(int y = 0 ; y < height ; y++)
            if (map.cellAt(x, y) == CellType::Empty && hasFloorNext(x, y))
                map.cellAt(x, y) = CellType::Wall;

    return map;
}


Pattern generate_rectangle(int width, int height)
{
    assert(width > 0);
    assert(height > 0);

    // Process where will be (0, 0)
    int center_x = width / 2;
    int center_y = height / 2;

    // Add every cells in the pattern
    Pattern room;
    for (int x = 0 ; x < width ; x++)
        for (int y = 0 ; y < height ; y++)
            room.insert({x-center_x, y-center_y});

    return room;
}

Pattern generate_rectangle(int size)
{
    assert(size > 0);

    int medium_width = std::sqrt(size);
    float ratio = Random::uniform_float((2.f / 3.f), (4.f / 3.f));

    int width = std::max(2, static_cast<int>(ratio * medium_width));
    int height = std::max(2, size / width);
    return generate_rectangle(width, height);
}

Pattern generate_hallway(std::pair<int, int> cell1, std::pair<int, int> cell2)
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
        path.insert({x+1, y});
        path.insert({x, y+1});
        path.insert({x+1, y+1});
    }

    return path;
}

void cavestyle_patch(Pattern& pattern, int nb_additions)
{
    Pattern surrounding;

    // Function to add a new cell to surrounding, if it isn't already in the patern.
    auto addSurrounding = [&pattern, &surrounding](int x, int y) -> void
    {
        if (pattern.count({x, y}) == 0)
            surrounding.insert({x, y});
    };

    // Add cells around the first ones
    for (auto& cell : pattern)
    {
        int x = cell.first;
        int y = cell.second;
        addSurrounding(x+1, y);
        addSurrounding(x-1, y);
        addSurrounding(x, y+1);
        addSurrounding(x, y-1);
    }

    // Create noise
    for(int nb_cells = 0 ; nb_cells < nb_additions ; nb_cells++)
    {
        // Select a cell to insert
        auto selected = surrounding.begin();

        std::advance(selected, Random::uniform_int(0, surrounding.size()-1));
        pattern.insert(*selected);

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
}

Pattern generate_cave(int size)
{
    if (size < 0)
        size = 1;

    Pattern cells;

    cells.insert(std::make_pair(0, 0));
    cavestyle_patch(cells, size-1);

    return cells;
}

Pattern generate_banana(int avg_side)
{
    int x1 = Random::uniform_int(-avg_side/2, avg_side/2);
    int y1 = Random::uniform_int(-avg_side/2, avg_side/2);
    int x2 = Random::uniform_int(-avg_side/2, avg_side/2);
    int y2 = Random::uniform_int(-avg_side/2, avg_side/2);

    auto path1 = generate_hallway({0, 0}, {x1, y1});
    auto path2 = generate_hallway({0, 0}, {x2, y2});

    return merged_patterns({{0, 0}, {0, 0}}, {path1, path2});
}

Level generate(const GenerationMode &mode)
{
    // List of rooms.
    std::vector<Pattern> patterns;
    std::vector<std::pair<int, int>> positions;

    // Create rooms of random size
    for (int i_room = 0 ; i_room < mode.nb_rooms ; i_room++)
    {
        int room_size = Random::uniform_int(mode.room_min_size, mode.room_max_size);

        switch (mode.type)
        {
            case LevelType::Cave:
                patterns.push_back(generate_cave(room_size));
                break;

            case LevelType::Flat:
            default:
                patterns.push_back(generate_rectangle(room_size));
                break;
        }
    }

    // Position rooms
    int map_right = 0; // right position of the end of last room
    for (int i_room = 0 ; i_room < mode.nb_rooms ; i_room++)
    {
        int room_left = pattern_min_x(patterns[i_room]);
        int room_right = pattern_max_x(patterns[i_room]);
        int pos_x = 1 + mode.room_margin + map_right - room_left;

        map_right += 1 + mode.room_margin + (room_right - room_left);
        positions.push_back({pos_x, Random::uniform_int(-25, 25)});
    }

    // Add ways between rooms
    for (int i_room = 1 ; i_room < mode.nb_rooms ; i_room++)
    {
        auto hall_start = positions[i_room-1];
        auto hall_end = positions[i_room];
        switch (mode.type)
        {
            case LevelType::Cave:
                patterns.push_back(generate_hallway(hall_start, hall_end));
                positions.push_back(positions[i_room-1]);
                cavestyle_patch(patterns.back(), patterns.back().size());
                break;

            case LevelType::Flat:
            default:
                patterns.push_back(generate_hallway(hall_start, hall_end));
                positions.push_back(positions[i_room-1]);
                break;
        }
    }

    Pattern cells = merged_patterns(positions, patterns);

    // Add stairs in first and last map
    std::vector<std::shared_ptr<Entity>> entities;
    entities.push_back(std::make_shared<Entity>(
        EntityType::Stairs,
        Interaction::GoDown,
        sf::Vector2u(positions[0].first, positions[0].second),
        Direction::Right
    ));
    entities.push_back(std::make_shared<Entity>(
        EntityType::Stairs,
        Interaction::GoUp,
        sf::Vector2u(positions[mode.nb_rooms-1].first, positions[mode.nb_rooms-1].second),
        Direction::Left
    ));

    // Add monsters in other maps
    for (int i_map = 1 ; i_map+1 < mode.nb_rooms ; i_map++)
    {
        entities.push_back(std::make_shared<Character>(
            EntityType::Monster,
            Interaction::None,
            sf::Vector2u(positions[i_map].first, positions[i_map].second),
            Direction::Left, 3, 1
        ));
    }

    // Outputs result into the map
    Map map = map_of_pattern(normalized_pattern(cells, entities));

    return {map, entities};
}
