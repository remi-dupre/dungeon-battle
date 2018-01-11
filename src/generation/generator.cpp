#include "generator.hpp"

using namespace std::chrono_literals;


Generator::Generator()
{
    parameters.infinite = false;
    setFilledChunk(0, 0);
}

Generator::Generator(const GenerationMode& parameters) :
    parameters(parameters)
{
    if (parameters.infinite)
    {
        generating_thread = std::thread(&Generator::generationLoop, this);
    }
    else
    {
        addRooms(0, 0, parameters.nb_rooms);
        setFilledChunk(0, 0);
    }
}

Chunk Generator::getChunkCells(int x, int y)
{
    if (parameters.infinite && !isLockedChunk(x, y)) {
        // Only generate this chunk
        generateRadius(x, y, 0);
    }

    setLockedChunk(x, y);
    std::lock_guard<std::mutex> lock(cache_lock);

    // Filter interesting cells and entities
    if (cached_map.hasChunk(x, y))
        return cached_map.chunkAt(x, y);
    else
        return Chunk();
}

std::vector<std::pair<int, int>> Generator::getCachedChunks()
{
    std::vector<std::pair<int, int>> ret;

    for (const auto& item: cached_map.getChunks())
    {
        if (!isLockedChunk(item.first, item.second))
            ret.push_back(item);
    }

    return ret;
}

std::vector<std::shared_ptr<Entity>> Generator::getChunkEntities(int x, int y)
{
    if (parameters.infinite && !isLockedChunk(x, y)) {
        // Only generate this chunk
        generateRadius(x, y, 0);
    }

    setLockedChunk(x, y);
    std::vector<std::shared_ptr<Entity>> copy;
    std::lock_guard<std::mutex> lock(cache_lock);

    if (cached_entities.count({x, y}))
        for (auto& entity : cached_entities[{x, y}])
            copy.push_back(entity->copy());

    return copy;
}

void Generator::preGenerateRadius(int x, int y, int radius, bool priority)
{
    assert(radius >= 0);

    if (!parameters.infinite)
        return;

    std::lock_guard<std::mutex> lock(to_generate_lock);

    // Radius of generated chunks
    int gen_radius = radius + GEN_BORDER;
    auto task = spiral(x, y, gen_radius);

    // We really want to generate the center first
    if (priority)
        std::reverse(begin(task), end(task));

    for (auto& chunk_id: task)
    {
        if (priority)
            to_generate.push_front(chunk_id);
        else
            to_generate.push_back(chunk_id);
    }
}

void Generator::generateRadius(int x, int y, int radius)
{
    assert(radius >= 0);

    if (!parameters.infinite)
        return;

    preGenerateRadius(x, y, radius, true);

    // Radius of generated chunks
    int gen_radius = radius + GEN_BORDER;

    for (int nx = x - gen_radius ; nx <= x + gen_radius ; nx++)
        for (int ny = y - gen_radius ; ny <= y + gen_radius ; ny++)
            while (!isFilledChunk(nx, ny))
                std::this_thread::sleep_for(10ms);
}

bool Generator::isLockedChunk(int x, int y)
{
    std::lock_guard<std::mutex> lock(locked_lock);
    return locked.find({x, y}) != end(locked);
}

void Generator::setLockedChunk(int x, int y)
{
    std::lock_guard<std::mutex> lock(locked_lock);
    locked.insert({x, y});
}

bool Generator::isFilledChunk(int x, int y)
{
    std::lock_guard<std::mutex> lock(filled_lock);
    return filled.find({x, y}) != end(filled);
}

void Generator::setFilledChunk(int x, int y)
{
    std::lock_guard<std::mutex> lock(filled_lock);
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
            sf::Vector2i(1, 0),
            Direction::Left
        ));
    }

    // Place monsters after other entities
    for (size_t i_room = rooms.size() - n ; i_room < rooms.size() ; i_room++)
        add_monsters(rooms[i_room], parameters.monster_load);

    // Copy rooms to the cached map and entities
    for (size_t i_room = rooms.size() - n ; i_room < rooms.size() ; i_room++)
    registerRoom(i_room);

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
    std::lock_guard<std::mutex> lock(cache_lock);

    // Add floor everywhere we can
    for (Point cell : rooms[room].getCells())
    {
        int x = (cell + rooms[room].getPosition()).first;
        int y = (cell + rooms[room].getPosition()).second;

        if (!cached_map.hasCell(x, y)) {
            auto cpos = Chunk::sector(x, y);
            cached_map.setChunk(cpos.first, cpos.second, Chunk());
        }

        cached_map.cellAt(x, y) = CellType::Floor;
    }

    // Add walls on the surrounding : only where there is no floor yet
    for (Point cell : surrounding(rooms[room].getCells()))
    {
        int x = (cell + rooms[room].getPosition()).first;
        int y = (cell + rooms[room].getPosition()).second;

        if (!cached_map.hasCell(x, y)) {
            auto cpos = Chunk::sector(x, y);
            cached_map.setChunk(cpos.first, cpos.second, Chunk());
        }

        if (cached_map.cellAt(x, y) == CellType::Empty)
            cached_map.cellAt(x, y) = CellType::Wall;
    }

    // Add entities in the cache
    for (auto entity : rooms[room].getEntities())
    {
        auto position = std::make_pair(entity->getPosition().x, entity->getPosition().y);
        int x = (position + rooms[room].getPosition()).first;
        int y = (position + rooms[room].getPosition()).second;

        auto chunk_id = Chunk::sector(x, y);
        cached_entities[chunk_id].push_back(entity->copy());
        cached_entities[chunk_id].back()->setPosition({x, y});
    }
}

void Generator::generationLoop()
{
    while (true)
    {
        to_generate_lock.lock();

        // Wait for at least one task
        while (to_generate.empty())
        {
            to_generate_lock.unlock();
            std::this_thread::sleep_for(10ms);
            to_generate_lock.lock();
        }

        std::pair<int, int> chunk_id;

        // Select a chunk to generate
        chunk_id = to_generate.front();
        to_generate.pop_front();

        to_generate_lock.unlock();

        if (isFilledChunk(chunk_id.first, chunk_id.second))
            continue;

        addRooms(chunk_id.first, chunk_id.second, 1);
        setFilledChunk(chunk_id.first, chunk_id.second);
    }
}
