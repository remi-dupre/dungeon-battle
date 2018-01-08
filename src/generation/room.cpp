#include "room.hpp"


Room::Room() :
    position({0, 0}),
    nodes({{0, 0}})
{}

Room merged_rooms(const std::vector<Room>& rooms)
{
    Room merged;

    for (size_t i_room = 0 ; i_room < rooms.size() ; i_room++)
    {
        for (const Point& cell : rooms[i_room].cells)
            merged.cells.insert(cell + rooms[i_room].position);

        for (auto entity : rooms[i_room].entities)
        {
            int x = entity->getPosition().x + rooms[i_room].position.first;
            int y = entity->getPosition().y + rooms[i_room].position.second;

            entity->setPosition({x, y});
            merged.entities.push_back(entity);
        }
    }

    return merged;
}

std::pair<Point, Point> closest_nodes(const Room& room1, const Room& room2)
{
    assert(!room1.nodes.empty());
    assert(!room2.nodes.empty());

    int best_dist = std::numeric_limits<int>::max();
    std::pair<int, int> best_cell1({0, 0});
    std::pair<int, int> best_cell2({0, 0});

    for (const Point& cell1 : room1.nodes)
    {
        for (const Point& cell2 : room2.nodes)
        {
            Point diff = room1.position + cell1 - cell2 - room2.position;
            int dist = std::abs(diff.first) + std::abs(diff.second);

            if (dist < best_dist)
            {
                best_dist = dist;
                best_cell1 = cell1;
                best_cell2 = cell2;
            }
        }
    }

    return std::make_pair(best_cell1, best_cell2);
}

int ntn_dist(const Room& room1, const Room& room2)
{
    assert(!room1.nodes.empty());
    assert(!room2.nodes.empty());

    Point cell1, cell2;
    std::tie(cell1, cell2) = closest_nodes(room1, room2);

    std::pair<int, int> diff = room1.position + cell1 - cell2 - room2.position;
    return std::abs(diff.first) + std::abs(diff.second);
}

void separate_rooms(std::vector<Room>& rooms, int spacing)
{
    assert(spacing >= 0);

    size_t nb_rooms = rooms.size();
    // Preprocess an efficient structure to calculate intersections
    std::vector<KDTree> dist_engines;
    for (size_t i_room = 0 ; i_room < nb_rooms ; i_room++)
        dist_engines.push_back(KDTree(rooms[i_room].cells));


    bool go_on = true; // Set to true while we changed something
    while(go_on)
    {
        go_on = false;
        std::vector<std::pair<int, int>> direction(nb_rooms, {0, 0});

        for (size_t i1 = 0 ; i1 < nb_rooms ; i1++)
        {
            for (size_t i2 = 0 ; i2 < i1 ; i2++)
            {
                // Takes a distinct pair of rooms.
                Room& room1 = rooms[i1];
                Room& room2 = rooms[i2];

                // Check if the two sets are close to each other
                bool well_spaced = true;
                for (auto& cell : rooms[i2].cells)
                    well_spaced = well_spaced && !dist_engines[i1].closeTo(cell + rooms[i2].position - rooms[i1].position, spacing);

                if (!well_spaced)
                {
                    go_on = true;

                    if (room1.position.first > room2.position.first)
                    {
                        int shift = std::max(1.f, (room1.position.first - room2.position.first + spacing) / 3.f);
                        direction[i1] += {shift, 0};
                    }

                    if (room1.position.second > room2.position.second)
                    {
                        int shift = std::max(1.f, (room1.position.second - room2.position.second + spacing) / 3.f);
                        direction[i1] += {0, shift};
                    }

                    if (room1.position.first < room2.position.first)
                    {
                        int shift = std::max(1.f, (room2.position.first - room1.position.first + spacing) / 3.f);
                        direction[i2] += {shift, 0};
                    }

                    if (room1.position.second < room2.position.second)
                    {
                        int shift = std::max(1.f, (room2.position.second - room1.position.second + spacing) / 3.f);
                        direction[i2] += {0, shift};
                    }

                    if (room1.position == room2.position)
                    {
                        // Moves room2 in a random direction.
                        int delta_x = Rand::uniform_int(0, 1);
                        int delta_y = Rand::uniform_int(0, 1);
                        direction[i1] += {2*delta_x-1, 2*delta_y-1};
                            go_on = true;
                    }
                }
            }
        }

        // Apply the position modifiers
        for (size_t i_room = 0 ; i_room < nb_rooms ; i_room++)
            rooms[i_room].position += direction[i_room];
    }
}

Room normalized_room(const Room& room)
{
    // Removes 1 to add a margin
    int min_x = pattern_min_x(room.cells) - 1;
    int min_y = pattern_min_y(room.cells) - 1;

    Room normalized;

    for (const Point& cell : room.cells)
    {
        assert(cell.first  - min_x >= 0);
        assert(cell.second - min_y >= 0);
        normalized.cells.insert({cell.first - min_x, cell.second - min_y});
    }

    for (const Point& cell : room.nodes)
    {
        assert(cell.first  - min_x >= 0);
        assert(cell.second - min_y >= 0);
        normalized.nodes.insert({cell.first - min_x, cell.second - min_y});
    }

    for (auto entity : room.entities)
    {
        auto pos = entity->getPosition();
        entity->setPosition({pos.x - min_x, pos.y - min_y});
        normalized.entities.push_back(entity);
    }

    return normalized;
}


void add_monsters(Room& room, float load)
{
    assert(load >= 0.f);
    assert(load <= 100.f);

    // Adds load/100 * size monsters in mean
    size_t nb_monsters = std::floor(room.cells.size() * load / 100.f);
    float excess = (room.cells.size() * load / 100.f) - nb_monsters;
    nb_monsters += (Rand::uniform_int(0, 1) < excess) ? 1 : 0;

    // Select forbidden cells
    Pattern forbidden;
    for (auto entity : room.entities)
        forbidden.insert(std::make_pair(entity->getPosition().x, entity->getPosition().y));

    // Process cells we could place monsters on
    std::vector<std::pair<int, int>> candidates;
    for (const Point& cell : room.cells)
        if (forbidden.find(cell) == end(forbidden))
            candidates.push_back(cell);
    nb_monsters = std::min(nb_monsters, candidates.size());

    // Select nb_monsters's indexes among all candidates
    std::random_shuffle(begin(candidates), end(candidates));

    for (size_t i_chosen = 0 ; i_chosen < nb_monsters ; i_chosen++)
    {
        // Selected cell of index indexes[i_chosen]
        Point& cell = candidates[i_chosen];
        room.entities.push_back(std::make_shared<Character>(
            randomClass(),
            sf::Vector2i(cell.first, cell.second)
        ));
    }
}

Room maze_room(int width, int height)
{
    Room room;
    room.cells = generate_maze(width, height);

    int max_x = pattern_max_x(room.cells);
    int max_y = pattern_max_y(room.cells);

    // Create relevant nodes
    room.nodes = Pattern();

    for (const Point& cell : room.cells)
        if (cell.first == 0 || cell.second == 0 || cell.first == max_x || cell.second == max_y)
            room.nodes.insert(cell);

    return room;
}
