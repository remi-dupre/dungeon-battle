#include "generator.hpp"


Generator::Generator(const GenerationMode& parameters, int seed) :
    parameters(parameters),
    seed(seed)
{
    RandGen::seed(seed);
}

Chunk Generator::getChunkCells(int x, int y)
{
    // Generate potential new rooms
    if (!parameters.infinite && built.empty()) {
        // Need to generate the whole map
        addRooms(0, 0, parameters.nb_rooms);
        built.insert({0, 0});
    }
    else if (parameters.infinite) {
        // Only generate this chunk
        generateRadius(x, y, 0);
    }

    // Filter interesting cells and entities
    if (cachedMap.hasChunk(x, y))
        return cachedMap.chunkAt(x, y);
    else
        return Chunk();
}

std::vector<std::shared_ptr<Entity>> Generator::getChunkEntities(int x, int y)
{
    // Generate potential new rooms
    if (!parameters.infinite && built.empty()) {
        // Need to generate the whole map
        addRooms(0, 0, parameters.nb_rooms);
        built.insert({0, 0});
    }
    else if (parameters.infinite) {
        // Only generate this chunk
        generateRadius(x, y, 0);
    }

    // Query on entities
    std::vector<std::shared_ptr<Entity>> ret;
    for (const Room& room : rooms)
    {
        for (std::shared_ptr<Entity> entity : room.entities)
        {
            Point cell = room.position + std::make_pair(entity->getPosition().x, entity->getPosition().y);
            Point chunk_id = Chunk::sector(cell.first, cell.second);

            if (chunk_id == std::make_pair(x, y)) {
                ret.push_back(entity->copy());
                ret.back()->setPosition({cell.first, cell.second});
            }
        }
    }

    return ret;
}

void Generator::generateRadius(int x, int y, int radius)
{
    assert(radius >= 0);
    assert(parameters.infinite);

    for (int nx = x - radius ; nx <= x + radius ; nx++)
    {
        for (int ny = y - radius ; ny <= y + radius ; ny++)
        {
            if (built.find({nx, ny}) == end(built))
            {
                std::cout << rooms.size() << std::endl;
                addRooms(nx, ny, 1);
                built.insert({nx, ny});
                build_order.push_back({nx, ny});
            }
        }
    }
}

void Generator::addRooms(int x, int y, int n)
{
    assert(n >= 0);

    // Create rooms of random size
    for (int i_room = 0 ; i_room < n ; i_room++)
    {
        Room room;

        int room_size = RandGen::uniform_int(parameters.room_min_size, parameters.room_max_size);

        float dice = RandGen::uniform_int(0, 9);
        if (dice == 0) // generate a maze
        {
            room = maze_room(23, 23);
        }
        else
        {
            switch (parameters.type)
            {
                case LevelType::Cave:
                    room.cells = generate_cave(room_size);
                    room.nodes = frontier(room.cells);
                    break;

                case LevelType::Flat:
                default:
                    room.cells = generate_maze(23, 23);
                    room.cells = generate_rectangle(room_size);
                    room.nodes = frontier(room.cells);
                    break;
            }
        }

        // Place the room at the center of given chunk
        room.position = std::make_pair((2*x + 1) * Chunk::SIZE / 2, (2*y + 1) * Chunk::SIZE / 2);

        // Add monsters on the room
        add_monsters(room, parameters.monster_load);

        rooms.push_back(room);
    }

    // Places rooms in a non-linear way
    separate_rooms(rooms, parameters.room_margin, rooms.size() - n, rooms.size());

    // Copy rooms to the cached map
    for (size_t i_room = rooms.size() - n ; i_room < rooms.size() ; i_room++)
        registerRoom(i_room);

    if (built.empty())
    {
        // Add entrance in first map
        rooms[0].entities.push_back(std::make_shared<Entity>(
            EntityType::Stairs,
            Interaction::GoUp,
            sf::Vector2i(0, 0),
            Direction::Right
        ));
    }

    if (!parameters.infinite)
    {
        // Add exit in last map (differs with the map containing entrance)
        rooms[n-1].entities.push_back(std::make_shared<Entity>(
            EntityType::Stairs,
            Interaction::GoDown,
            sf::Vector2i(0, 0),
            Direction::Left
        ));
    }

    // Add ways between rooms
    updateLinks();
}

void Generator::updateLinks()
{
    size_t nb_rooms = rooms.size();

    // Create a union-find representing connections
    std::vector<size_t> size_uf(nb_rooms, 1);
    std::vector<size_t> link_uf(nb_rooms);
    for (size_t i = 0 ; i < nb_rooms ; i++)
        link_uf[i] = i;

    // Access function for the union find
    std::function<size_t(size_t)> comp_repr = [&comp_repr, &link_uf] (size_t i_room) -> size_t
    {
        assert(i_room < link_uf.size());

        if (link_uf[i_room] == i_room)
            return i_room;

        size_t repr = comp_repr(link_uf[i_room]);
        link_uf[i_room] = repr;
        return repr;
    };

    // Function to merge two components of the union-find
    auto comp_merge = [comp_repr, &link_uf, &size_uf](size_t a, size_t b)
    {
        assert(a < link_uf.size());
        assert(b < link_uf.size());

        if (comp_repr(a) != comp_repr(b))
        {
            size_uf[comp_repr(a)] = size_uf[comp_repr(a)] + size_uf[comp_repr(b)];
            link_uf[comp_repr(b)] = comp_repr(a);
        }
    };

    // Update the union find with current links
    for (auto &link : room_links)
        comp_merge(link.first, link.second);

    // Create vertices (dist(i, j), i, j) in an increasing order if i and j are not in the same component yet
    typedef std::tuple<int, size_t, size_t> Edge;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> candidates;

    for (size_t i = 0 ; i < nb_rooms ; i++)
    {
        for (size_t j = 0 ; j < i ; j++)
        {
            if (comp_repr(i) != comp_repr(j))
            {
                int dist = ntn_dist(rooms[i], rooms[j]);
                candidates.push(std::make_tuple(dist, i, j));
            }
        }
    }

    // Force connexity of the level
    while (size_uf[comp_repr(0)] < nb_rooms)
    {
        int dist;
        size_t l, r;
        std::tie(dist, l, r) = candidates.top();
        candidates.pop();

        // If these two rooms are not linked yet
        if (comp_repr(l) != comp_repr(r))
        {
            // Add path between the two rooms
            auto close_points = closest_nodes(rooms[l], rooms[r]);
            Point hall_start = close_points.first + rooms[l].position;
            Point hall_end = close_points.second + rooms[r].position;

            Room path;
            path.position = hall_start;
            switch (parameters.type)
            {
                case LevelType::Cave:
                    path.cells = generate_hallway(hall_start, hall_end);
                    cavestyle_patch(path.cells, path.cells.size());
                    path.nodes = frontier(path.cells);
                    break;

                case LevelType::Flat:
                default:
                    path.cells = generate_hallway(hall_start, hall_end);
                    path.nodes = frontier(path.cells);
                    break;
            }
            rooms.push_back(path);

            room_links.insert({l, nb_rooms});
            room_links.insert({nb_rooms, r});
            registerRoom(nb_rooms);

            // Update union-find
            nb_rooms++;
            assert(nb_rooms == rooms.size());

            link_uf.push_back(nb_rooms-1);
            size_uf.push_back(1);

            comp_merge(l, nb_rooms-1);
            comp_merge(nb_rooms-1, r);

            // Insert new possible distances to the new path
            for (size_t i = 0 ; i < nb_rooms-1 ; i++)
            {
                int dist = ntn_dist(rooms[i], rooms[nb_rooms-1]);
                candidates.push(std::make_tuple(dist, i, nb_rooms-1));
            }
        }
    }
}

void Generator::registerRoom(size_t room)
{
    // Add floor everywhere we can
    for (Point cell : rooms[room].cells)
    {
        int x = (cell + rooms[room].position).first;
        int y = (cell + rooms[room].position).second;

        if (!cachedMap.hasCell(x, y)) {
            auto cpos = Chunk::sector(x, y);
            cachedMap.setChunk(cpos.first, cpos.second, Chunk());
        }

        cachedMap.cellAt(x, y) = CellType::Floor;
    }

    // Add walls on the surrounding : only where there is no floor yet
    for (Point cell : surrounding(rooms[room].cells))
    {
        int x = (cell + rooms[room].position).first;
        int y = (cell + rooms[room].position).second;

        if (!cachedMap.hasCell(x, y)) {
            auto cpos = Chunk::sector(x, y);
            cachedMap.setChunk(cpos.first, cpos.second, Chunk());
        }

        if (cachedMap.cellAt(x, y) == CellType::Empty)
            cachedMap.cellAt(x, y) = CellType::Wall;
    }
}
