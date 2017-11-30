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
    if (!parameters.infinite && requested.empty()) {
        // Need to generate the whole map
        addRooms(0, 0, parameters.nb_rooms);
    }
    else if (parameters.infinite) {
        assert(false);
        // ...
    }

    // Filter interesting cells and entities
    Chunk chunk;

    for (const Room& room : rooms)
    {
        for (Point cell : room.cells)
        {
            cell += room.position;

            Point chunk_id = Chunk::sector(cell.first, cell.second);
            Point relative_pos = Chunk::relative(cell.first, cell.second);

            if (chunk_id == std::make_pair(x, y)) {
                chunk.cellAt(relative_pos.first, relative_pos.second) = CellType::Floor;
            }
        }
    }

    requested.insert(std::make_pair(x, y));
    return chunk;
}
#include <iostream>
std::vector<std::shared_ptr<Entity>> Generator::getChunkEntities(int x, int y)
{
    // Generate potential new rooms
    if (!parameters.infinite && requested.empty()) {
        // Need to generate the whole map
        addRooms(0, 0, parameters.nb_rooms);
    }
    else if (parameters.infinite) {
        assert(false);
        // ...
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
                std::cout << "-----" << std::endl;
                std::cout << room.position.first << ' ' << room.position.second << std::endl;
                std::cout << entity->getPosition().x << ' ' << entity->getPosition().y << std::endl;
                std::cout << cell.first << ' ' << cell.second << std::endl;

                ret.push_back(entity->copy());
                ret.back()->setPosition({cell.first, cell.second});
            }
        }
    }

    return ret;
}

void Generator::addRooms(int x, int y, size_t n)
{
    // /!\ for now, only generate for finite maps
    // List of rooms.
    rooms = std::vector<Room>(n);

    // Create rooms of random size
    for (Room& room : rooms)
    {
        int room_size = RandGen::uniform_int(parameters.room_min_size, parameters.room_max_size);

        float dice = RandGen::uniform_int(0, 9);
        if (dice == 0) // generate a maze
        {
            room = maze_room(23, 23);
            continue;
        }

        switch (parameters.type)
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

        // Place the room at the center of given chunk
        room.position = std::make_pair((2*x + 1) * Chunk::SIZE / 2, (2*y + 1) * Chunk::SIZE / 2);
    }

    // Places rooms in a non-linear way
    separate_rooms(rooms, parameters.room_margin);

    // Add stairs in first and last map
    rooms[0].entities.push_back(std::make_shared<Entity>(
        EntityType::Stairs,
        Interaction::GoDown,
        sf::Vector2i(0, 0),
        Direction::Right
    ));
    rooms[n-1].entities.push_back(std::make_shared<Entity>(
        EntityType::Stairs,
        Interaction::GoUp,
        sf::Vector2i(0, 0),
        Direction::Left
    ));

    // Add monsters
    for (Room& room : rooms)
        add_monsters(room, parameters.monster_load);

    // Add ways between rooms
    room_links = covering_paths(rooms);
    for (auto edge : room_links)
    {
        // Create a pattern, placed at the first node position's
        Room path;

        std::pair<Point, Point> close_points = closest_nodes(rooms[edge.first], rooms[edge.second]);
        Point hall_start = close_points.first + rooms[edge.first].position;
        Point hall_end = close_points.second + rooms[edge.second].position;

        path.position = hall_start;

        switch (parameters.type)
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

        rooms.push_back(path);
    }
}
