#include "level.hpp"


std::vector<std::pair<size_t, size_t>> covering_paths(const std::vector<Room>& rooms)
{
    size_t nb_rooms = rooms.size();

    // Resulting graph
    std::vector<std::pair<size_t, size_t>> edges;

    // Create a union-find representing connections
    std::vector<size_t> link_uf(nb_rooms);
    std::vector<size_t> size_uf(nb_rooms, 1);
    for (size_t i = 0 ; i < nb_rooms ; i++)
        link_uf[i] = i;

    // Access function for the union find
    std::function<size_t(size_t)> comp_repr = [&comp_repr, &link_uf] (size_t i_room) {
        if (link_uf[i_room] == i_room)
            return i_room;

        size_t repr = comp_repr(link_uf[i_room]);
        link_uf[i_room] = repr;
        return repr;
    };

    // Create vertices (dist(i, j), i, j) in an increasing order
    typedef std::tuple<int, size_t, size_t> Edge;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> candidates;

    for (size_t i = 0 ; i < nb_rooms ; i++)
    {
        for (size_t j = 0 ; j < i ; j++)
        {
            int dist = ntn_dist(rooms[i], rooms[j]);
            candidates.push(std::make_tuple(dist, i, j));
        }
    }

    // Waits for rooms to be all linked
    while (size_uf[comp_repr(0)] < nb_rooms)
    {
        int dist;
        size_t l, r;
        std::tie(dist, l, r) = candidates.top();

        edges.push_back({l, r});

        if (comp_repr(l) != comp_repr(r))
        {
            // Make union of two components
            size_uf[comp_repr(l)] = size_uf[comp_repr(l)] + size_uf[comp_repr(r)];
            link_uf[comp_repr(r)] = comp_repr(l);
        }

        candidates.pop();
    }

    return edges;
}

Map map_of_pattern(const Pattern& pattern)
{
    // The coordinates are from 0 to pattern_max_?
    // Add a 1-unit margin at top and right border
    int width = pattern_max_x(pattern) + 2;
    int height = pattern_max_y(pattern) + 2;
    Map map(width, height);

    // Add cells of the pattern on the floor
    for (const Point& cell : pattern)
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
    std::vector<Room> rooms(mode.nb_rooms);

    // Create rooms of random size
    for (Room& room : rooms)
    {
        int room_size = Rand::uniform_int(mode.room_min_size, mode.room_max_size);

        float dice = Rand::uniform_int(0, 9);
        if (dice == 0) // generate a maze
        {
            room = maze_room(23, 23);
            continue;
        }

        switch (mode.type)
        {
            case LevelType::Cave:
                room.cells = generate_cave(room_size);
                room.nodes = surrounding(room.cells);
                break;

            case LevelType::Flat:
            default:
                room.cells = generate_maze(23, 23);
                room.cells = generate_rectangle(room_size);
                room.nodes = surrounding(room.cells);
                break;
        }
    }

    // Places rooms in a non-linear way
    separate_rooms(rooms, mode.room_margin);

    // Add stairs in first and last map
    rooms[0].entities.push_back(std::make_shared<Entity>(
        EntityType::Stairs,
        Interaction::GoDown,
        sf::Vector2i(0, 0),
        Direction::Right
    ));
    rooms[mode.nb_rooms-1].entities.push_back(std::make_shared<Entity>(
        EntityType::Stairs,
        Interaction::GoUp,
        sf::Vector2i(0, 0),
        Direction::Left
    ));

    // Add monsters
    for (Room& room : rooms)
        add_monsters(room, mode.monster_load);

    // Add ways between rooms
    auto edges = covering_paths(rooms);
    for (auto edge : edges)
    {
        // Create a pattern, placed at the first node position's
        rooms.push_back(Room());
        Room& path = rooms.back();

        std::pair<Point, Point> close_points = closest_nodes(rooms[edge.first], rooms[edge.second]);
        Point hall_start = close_points.first + rooms[edge.first].position;
        Point hall_end = close_points.second + rooms[edge.second].position;

        path.position = hall_start;

        switch (mode.type)
        {
            case LevelType::Cave:
                path.cells = generate_hallway(hall_start, hall_end);
                cavestyle_patch(path.cells, path.cells.size());
                break;

            case LevelType::Flat:
            default:
                path.cells = generate_hallway(hall_start, hall_end);
                break;
        }
    }


    // Outputs result into the map
    Room full_level = normalized_room(merged_rooms(rooms));
    Map map = map_of_pattern(full_level.cells);

    return {std::move(map), std::move(full_level.entities)};
}
