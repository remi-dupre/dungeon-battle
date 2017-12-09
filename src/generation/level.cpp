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
    Map map;

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
