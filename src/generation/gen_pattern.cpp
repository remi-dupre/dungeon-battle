#include "gen_pattern.hpp"


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
    float ratio = Rand::uniform_float((2.f / 3.f), (4.f / 3.f));

    int width = std::max(2, static_cast<int>(ratio * medium_width));
    int height = std::max(2, size / width);
    return generate_rectangle(width, height);
}

Pattern generate_maze(int width, int height)
{
    assert(width % 2 == 1);
    assert(height % 2 == 1);

    // Create a graph representing the maze
    int maze_width = width / 2;
    int maze_height = height / 2;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> maze_graph;
    std::vector<std::vector<bool>> visited(maze_width, std::vector<bool>(maze_height, false));

    // Run the maze, adding new nodes
    std::function<void(int, int)> run = [maze_width, maze_height, &run, &maze_graph, &visited](int x, int y)
    {
        std::pair<int, int> pos = std::make_pair(x, y);
        visited[x][y] = true;

        std::vector<std::pair<int, int>> directions = {{0, +1}, {0, -1}, {+1, 0}, {-1, 0}};
        std::random_shuffle(begin(directions), end(directions));

        for (auto direction : directions)
        {
            auto new_pos = pos + direction;
            if(new_pos.first >= 0 && new_pos.first < maze_width
                && new_pos.second >= 0 && new_pos.second < maze_height
                && !visited[new_pos.first][new_pos.second])
            {
                maze_graph.push_back({pos, new_pos});
                run(new_pos.first, new_pos.second);
            }
        }
    };

    run(Rand::uniform_int(0, maze_width-1), Rand::uniform_int(0, maze_height-1));

    // Build the cells of the graph
    Pattern cells;
    for (auto& link : maze_graph)
    {
        int x1, x2, y1, y2;
        std::tie(x1, y1) = link.first;
        std::tie(x2, y2) = link.second;

        cells.insert({x1*2, y1*2});
        cells.insert({x2*2, y2*2});
        cells.insert({x1+x2, y1+y2});
    }

    return cells;
}

Pattern generate_hallway(std::pair<int, int> cell1, std::pair<int, int> cell2)
{
    int x1, y1, x2, y2;
    std::tie(x1, y1) = cell1;
    std::tie(x2, y2) = cell2;

    int x = 0, y = 0;
    Pattern path;
    path.insert({0, 0});

    while (x1 + x != x2 || y1 + y != y2)
    {
        path.insert({x, y});
        path.insert({x+1, y});
        path.insert({x, y+1});
        path.insert({x+1, y+1});

        if (x1 + x < x2)
            x++;
        if (x1 + x > x2)
            x--;
        if (y1 + y < y2)
            y++;
        if (y1 + y > y2)
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

    // Function to add a new cell to surrounding, if it isn't already in the pattern.
    auto addSurrounding = [&pattern, &surrounding](std::pair<int, int> cell)
    {
        if (pattern.count(cell) == 0)
            surrounding.insert(cell);
    };

    // Add cells around the first ones
    for (auto& cell : pattern)
    {
        addSurrounding(cell + std::make_pair(1, 0));
        addSurrounding(cell + std::make_pair(0, 1));
        addSurrounding(cell - std::make_pair(1, 0));
        addSurrounding(cell - std::make_pair(0, 1));
    }

    // Create noise
    for(int nb_cells = 0 ; nb_cells < nb_additions ; nb_cells++)
    {
        // Select a cell to insert
        auto selected = surrounding.begin();

        std::advance(selected, Rand::uniform_int(0, surrounding.size()-1));
        pattern.insert(*selected);

        // Refresh surrounding set of the pattern
        addSurrounding((*selected) + std::make_pair(1, 0));
        addSurrounding((*selected) + std::make_pair(0, 1));
        addSurrounding((*selected) - std::make_pair(1, 0));
        addSurrounding((*selected) - std::make_pair(0, 1));

        // Can't select this cell anymore
        surrounding.erase(selected);
    }
}

Pattern generate_cave(int size)
{
    if (size < 0)
        size = 1;

    Pattern cells;
    cells.insert({0, 0});
    cavestyle_patch(cells, size-1);
    return cells;
}

Pattern generate_banana(int avg_side)
{
    int x1 = Rand::uniform_int(-avg_side/2, avg_side/2);
    int y1 = Rand::uniform_int(-avg_side/2, avg_side/2);
    int x2 = Rand::uniform_int(-avg_side/2, avg_side/2);
    int y2 = Rand::uniform_int(-avg_side/2, avg_side/2);

    auto path1 = generate_hallway({0, 0}, {x1, y1});
    auto path2 = generate_hallway({0, 0}, {x2, y2});

    return merged_patterns({{0, 0}, {0, 0}}, {path1, path2});
}
