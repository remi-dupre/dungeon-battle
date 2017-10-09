#include "level.hpp"


void separate_rooms(
    std::vector<std::pair<int, int>>& positions,
    const std::vector<Pattern>& rooms,
    int spacing)
{
    assert(spacing >= 0);
    assert(positions.size() == rooms.size());

    // The direction each rooms will be send to
    int nb_rooms = positions.size();

    // Preprocess the surrounding of rooms to reduce distance calculations
    std::vector<Pattern> frontiers(nb_rooms);
    for (int i_room = 0 ; i_room < nb_rooms ; i_room++)
        frontiers[i_room] = surrounding(rooms[i_room]);

    bool go_on = true; // Set to true while we changed something
    while(go_on)
    {
        go_on = false;
        std::vector<std::pair<int, int>> direction(nb_rooms, {0, 0});

        for (int i1 = 0 ; i1 < nb_rooms ; i1++)
        {
            for (int i2 = 0 ; i2 < i1 ; i2++)
            {
                // Takes a distinct pair of rooms.
                // Send them in a direction if they are not spaced enough.
                bool well_spaced =
                    superposed(positions[i1], rooms[i1], positions[i2], rooms[i2])
                    || !spaced(positions[i1], frontiers[i1], positions[i2], frontiers[i2], spacing);
                if (well_spaced)
                {
                    go_on = true;

                    if (positions[i1].first > positions[i2].first)
                    {
                        int shift = std::max(1.f, (positions[i1].first - positions[i2].first + spacing) / 3.f);
                        direction[i1] += {shift, 0};
                    }

                    if (positions[i1].second > positions[i2].second)
                    {
                        int shift = std::max(1.f, (positions[i1].second - positions[i2].second + spacing) / 3.f);
                        direction[i1] += {0, shift};
                    }

                    if (positions[i1].first < positions[i2].first)
                    {
                        int shift = std::max(1.f, (positions[i2].first - positions[i1].first + spacing) / 3.f);
                        direction[i2] += {shift, 0};
                    }

                    if (positions[i1].second < positions[i2].second)
                    {
                        int shift = std::max(1.f, (positions[i2].second - positions[i1].second + spacing) / 3.f);
                        direction[i2] += {0, shift};
                    }

                    if (positions[i1] == positions[i2])
                    {
                        // Moves room2 in a random direction.
                        int delta_x = Random::uniform_int(0, 1);
                        int delta_y = Random::uniform_int(0, 1);
                        direction[i1] += {2*delta_x-1, 2*delta_y-1};
                            go_on = true;
                    }
                }
            }
        }

        // Apply the position modifiers
        for (int i_room = 0 ; i_room < nb_rooms ; i_room++)
            positions[i_room] += direction[i_room];
    }
}


Map map_of_pattern(const Pattern& pattern)
{
    // The coordinates are from 0 to pattern_max_?
    // Add a 1-unit margin at top and right border
    int width = pattern_max_x(pattern) + 2;
    int height = pattern_max_y(pattern) + 2;
    Map map(width, height);

    // Add cells of the pattern on the floor
    for (auto &cell : pattern)
    {
        int x = cell.first;
        int y = cell.second;

        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        map.cellAt(x, y) = CellType::Floor;
    }

    // Function to check wether a cell adjacent to (x, y) is a floor
    auto hasFloorNext = [height, width, &map](int x, int y)
    {
        for (int i = x-1 ; i <= x+1 ; i++)
            for (int j = y-1 ; j <= y+1 ; j++)
                if (i >= 0 && i < width && j >= 0 && j < height)
                    if ((i != x || j != y) && map.cellAt(i, j) == CellType::Floor)
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


Level generate(const GenerationMode &mode)
{
    // List of rooms.
    std::vector<Pattern> patterns;
    std::vector<std::pair<int, int>> positions;
    std::vector<std::vector<std::shared_ptr<Entity>>> room_monsters(mode.nb_rooms);

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

        add_monsters(patterns[i_room], room_monsters[i_room], mode.monster_load);
    }

    // Places rooms in a non-linear way
    while (positions.size() < patterns.size())
        positions.push_back({0, 0});
    separate_rooms(positions, patterns, mode.room_margin);

    // Position rooms
    // int map_right = 0; // right position of the end of last room
    // for (int i_room = 0 ; i_room < mode.nb_rooms ; i_room++)
    // {
    //     int room_left = pattern_min_x(patterns[i_room]);
    //     int room_right = pattern_max_x(patterns[i_room]);
    //     int pos_x = 1 + mode.room_margin + map_right - room_left;
    //
    //     map_right += 1 + mode.room_margin + (room_right - room_left);
    //     positions.push_back({pos_x, Random::uniform_int(-25, 25)});
    // }


    std::vector<std::shared_ptr<Entity>> entities = merged_entities(positions, room_monsters);

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

    // Verify that center of rooms are always filled
    for (auto pos : positions)
        assert(cells.find(pos) != std::end(cells));

    // Add stairs in first and last map
    entities.push_back(std::make_shared<Entity>(
        EntityType::Stairs,
        Interaction::GoDown,
        sf::Vector2i(positions[0].first, positions[0].second),
        Direction::Right
    ));
    entities.push_back(std::make_shared<Entity>(
        EntityType::Stairs,
        Interaction::GoUp,
        sf::Vector2i(positions[mode.nb_rooms-1].first, positions[mode.nb_rooms-1].second),
        Direction::Left
    ));

    // Add monsters in other maps
    // for (int i_map = 1 ; i_map+1 < mode.nb_rooms ; i_map++)
    // {
    //     entities.push_back(std::make_shared<Character>(
    //         EntityType::Monster,
    //         Interaction::None,
    //         sf::Vector2i(positions[i_map].first, positions[i_map].second),
    //         Direction::Left, 3, 1
    //     ));
    // }

    // Outputs result into the map
    cells = normalized_pattern(cells, entities);
    Map map = map_of_pattern(cells);

    return {map, entities};
}
