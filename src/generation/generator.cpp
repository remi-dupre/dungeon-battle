#include "generator.hpp"


Generator::Generator() :
    lockedLock(std::make_unique<std::mutex>()),
    filledLock(std::make_unique<std::mutex>())
{
    parameters.infinite = false;
    setFilledChunk(0, 0);
}

Generator::Generator(const GenerationMode& parameters) :
    parameters(parameters),
    lockedLock(std::make_unique<std::mutex>()),
    filledLock(std::make_unique<std::mutex>())
{}

Chunk Generator::getChunkCells(int x, int y)
{
    // Generate potential new rooms
    if (!parameters.infinite && filled.empty()) {
        // Need to generate the whole map
        addRooms(0, 0, parameters.nb_rooms);
        setFilledChunk(0, 0);
    }
    else if (parameters.infinite && !isLockedChunk(x, y)) {
        // Only generate this chunk
        generateRadius(x, y, 1);
    }

    setLockedChunk(x, y);

    // Filter interesting cells and entities
    if (cachedMap.hasChunk(x, y))
        return cachedMap.chunkAt(x, y);
    else
        return Chunk();
}

std::vector<std::pair<int, int>> Generator::getCachedChunks() const
{
    std::vector<std::pair<int, int>> ret;

    for (const auto& item: cachedMap.getChunks())
    {
        if (!isLockedChunk(item.first, item.second))
            ret.push_back(item);
    }

    return ret;
}

std::vector<std::shared_ptr<Entity>> Generator::getChunkEntities(int x, int y)
{
    // Generate potential new rooms
    if (!parameters.infinite && filled.empty()) {
        // Need to generate the whole map
        addRooms(0, 0, parameters.nb_rooms);
        setFilledChunk(0, 0);
    }
    else if (parameters.infinite && !isLockedChunk(x, y)) {
        // Only generate this chunk
        generateRadius(x, y, 1);
    }

    setLockedChunk(x, y);

    // Query on entities
    std::vector<std::shared_ptr<Entity>> ret;
    for (const Room& room : rooms)
    {
        for (std::shared_ptr<Entity> entity : room.getEntities())
        {
            Point cell = room.getPosition() + std::make_pair(entity->getPosition().x, entity->getPosition().y);
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

    // Radius of generated chunks
    int gen_radius = radius + GEN_BORDER;

    for (int radius_layer = 0 ; radius_layer <= gen_radius ; radius_layer++)
    {
        for (int nx = x - radius_layer ; nx <= x + radius_layer ; nx++)
        {
            for (int ny : std::set<int>({y - radius_layer, y + radius_layer})) {
                if (!isFilledChunk(nx, ny))
                {
                    addRooms(nx, ny, 1);
                    setFilledChunk(nx, ny);
                    build_order.push_back({nx, ny});
                }
            }
        }

        for (int ny = y - radius_layer + 1 ; ny < y + radius_layer ; ny++)
        {
            for (int nx : std::set<int>({x - radius_layer, x + radius_layer})) {
                if (!isFilledChunk(nx, ny))
                {
                    addRooms(nx, ny, 1);
                    setFilledChunk(nx, ny);
                    build_order.push_back({nx, ny});
                }
            }
        }
    }

    // Register some chunk being requested
    for (int nx = x - radius ; nx <= x + radius ; nx++)
        for (int ny = y - radius ; ny <= y + radius ; ny++)
            setLockedChunk(nx, ny);
}

bool Generator::isLockedChunk(int x, int y) const
{
    std::lock_guard<std::mutex> lock(*lockedLock);
    return locked.find({x, y}) != end(locked);
}

void Generator::setLockedChunk(int x, int y)
{
    std::lock_guard<std::mutex> lock(*lockedLock);
    locked.insert({x, y});
}

bool Generator::isFilledChunk(int x, int y) const
{
    std::lock_guard<std::mutex> lock(*filledLock);
    return filled.find({x, y}) != end(filled);
}

void Generator::setFilledChunk(int x, int y)
{
    std::lock_guard<std::mutex> lock(*filledLock);
    filled.insert({x, y});
}

void Generator::addRooms(int x, int y, int n)
{
    assert(n >= 0);

    // Create rooms of random size
    for (int i_room = 0 ; i_room < n ; i_room++)
    {
        Room room{Pattern()};
        Pattern cells;

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
                    cells = generate_cave(room_size);
                    break;

                case LevelType::Flat:
                default:
                    cells = generate_rectangle(room_size);
                    break;
            }

            room = Room(cells);
        }

        // Place the room at the center of given chunk
        room.setPosition({(2*x + 1) * Chunk::SIZE / 2, (2*y + 1) * Chunk::SIZE / 2});
        rooms.push_back(room);
    }

    if (rooms.size() > 1)
    {
        // Places rooms in a non-linear way, room of index 0 must not move
        separate_rooms(rooms, parameters.room_margin, std::max(1, (int) rooms.size() - n), rooms.size());

        // Remove rooms that colapse, not the one of index 0
        size_t i_room = std::max(1, (int) rooms.size() - n);
        while (i_room < rooms.size())
        {
            bool superposed = false;

            // Check if a room colapse with i
            for (size_t j_room = 0 ; j_room < i_room ; j_room++)
            {
                if (!spaced(rooms[i_room], rooms[j_room], 1))
                {
                    superposed = true;
                    break;
                }
            }

            if (superposed)
            {
                // Delete the room
                rooms.erase(begin(rooms) + i_room);
                n--;
            }
            else
            {
                // This room can be kept
                i_room++;

            }
        }
    }

    // Copy rooms to the cached map
    for (size_t i_room = rooms.size() - n ; i_room < rooms.size() ; i_room++)
        registerRoom(i_room);

    // Add stairs
    if (filled.empty())
    {
        // Add entrance in first map
        rooms[0].addEntity(std::make_shared<Entity>(
            EntityType::Stairs,
            Interaction::GoUp,
            sf::Vector2i(0, 0),
            Direction::Right
        ));
    }

    if (!parameters.infinite)
    {
        // Add exit in last map (differs with the map containing entrance)
            rooms[n-1].addEntity(std::make_shared<Entity>(
            EntityType::Stairs,
            Interaction::GoDown,
            sf::Vector2i(0, 0),
            Direction::Left
        ));
    }

    // Place monsters after other entities
    for (size_t i_room = rooms.size() - n ; i_room < rooms.size() ; i_room++)
        add_monsters(rooms[i_room], parameters.monster_load);

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
            Point hall_start = close_points.first + rooms[l].getPosition();
            Point hall_end = close_points.second + rooms[r].getPosition();

            Pattern path_cells;
            switch (parameters.type)
            {
                case LevelType::Cave:
                    path_cells = generate_hallway(hall_start, hall_end);
                    cavestyle_patch(path_cells, path_cells.size());
                    break;

                case LevelType::Flat:
                default:
                    path_cells = generate_hallway(hall_start, hall_end);
                    break;
            }

            Room path(path_cells);
            path.setPosition(hall_start);
            rooms.push_back(path);

            room_links.insert({l, nb_rooms});
            room_links.insert({nb_rooms, r});
            registerRoom(nb_rooms);

            // Update union-find
            nb_rooms++;
            assert(nb_rooms == rooms.size());

            link_uf.push_back(nb_rooms-1);
            size_uf.push_back(1);

            assert(l < link_uf.size());
            assert(r < link_uf.size());

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
    for (Point cell : rooms[room].getCells())
    {
        int x = (cell + rooms[room].getPosition()).first;
        int y = (cell + rooms[room].getPosition()).second;

        if (!cachedMap.hasCell(x, y)) {
            auto cpos = Chunk::sector(x, y);
            cachedMap.setChunk(cpos.first, cpos.second, Chunk());
        }

        cachedMap.cellAt(x, y) = CellType::Floor;
    }

    // Add walls on the surrounding : only where there is no floor yet
    for (Point cell : surrounding(rooms[room].getCells()))
    {
        int x = (cell + rooms[room].getPosition()).first;
        int y = (cell + rooms[room].getPosition()).second;

        if (!cachedMap.hasCell(x, y)) {
            auto cpos = Chunk::sector(x, y);
            cachedMap.setChunk(cpos.first, cpos.second, Chunk());
        }

        if (cachedMap.cellAt(x, y) == CellType::Empty)
            cachedMap.cellAt(x, y) = CellType::Wall;
    }
}
