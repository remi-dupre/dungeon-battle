#include "room.hpp"


/* *************** Definition of room *************** */

Room::Room() :
    position({0, 0}),
    treeCells(cells)
{}

Room::Room(const Pattern& cells) :
    position({0, 0}),
    cells(cells),
    nodes(frontier(cells)),
    treeCells(cells)
{}

Point Room::getPosition() const
{
    return position;
}

void Room::setPosition(const Point& nposition)
{
    position = nposition;
}

Pattern Room::getCells() const
{
    return cells;
}

int Room::size() const
{
    return cells.size();
}

bool Room::hasCell(const Point& cell) const
{
    return cells.find(cell - position) != end(cells);
}

Pattern Room::getNodes() const
{
    return nodes;
}

void Room::setNodes(const Pattern& nnodes)
{
    nodes = nnodes;
}

std::vector<std::shared_ptr<Entity>> Room::getEntities() const
{
    return entities;
}

void Room::addEntity(std::shared_ptr<Entity> entity)
{
    entities.push_back(entity);
}

bool spaced(const Room& room1, const Room& room2, int spacing)
{
    assert(spacing >= 1);

    if (room1.size() > room2.size())
        return spaced(room2, room1, spacing);

    if (room2.hasCell(room1.getPosition()))
        return false;

    for (const Point& cell : room1.cells)
        if (room2.treeCells.closeTo(cell + room1.position - room2.position, spacing-1))
            return false;

    return true;
}

std::ostream& operator<<(std::ostream& stream, const Room& room)
{
    stream << room.position;

    uint32_t nb_rooms = room.cells.size();
    uint32_t nb_nodes = room.nodes.size();
    uint32_t nb_entie = room.entities.size();

    stream.write(reinterpret_cast<char*>(&nb_rooms), sizeof(uint32_t));
    stream.write(reinterpret_cast<char*>(&nb_nodes), sizeof(uint32_t));
    stream.write(reinterpret_cast<char*>(&nb_entie), sizeof(uint32_t));

    for (const Point& cell: room.cells)
        stream << cell;

    for (const Point& cell: room.nodes)
        stream << cell;

    for (const auto& entity: room.entities)
    {
        if (entity->getType() == EntityType::Hero || entity->getType() == EntityType::Monster)
            stream << *std::static_pointer_cast<Character>(entity);
        else
            stream << *entity;
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, Room& room)
{
    stream >> room.position;

    room.cells.clear();
    room.nodes.clear();
    room.entities.clear();

    uint32_t nb_cells, nb_nodes, nb_entie;
    stream.read(reinterpret_cast<char*>(&nb_cells), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&nb_nodes), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&nb_entie), sizeof(uint32_t));

    for (size_t i = 0 ; i < nb_cells ; i++)
    {
        Point cell;
        stream >> cell;
        room.cells.insert(cell);
    }

    for (size_t i = 0 ; i < nb_nodes ; i++)
    {
        Point node;
        stream >> node;
        room.nodes.insert(node);
    }

    for (size_t i = 0 ; i < nb_entie ; i++)
    {
        EntityType type;
        stream.read(reinterpret_cast<char*>(&type), sizeof(uint32_t));
        stream.seekg(-static_cast<int>(sizeof(uint32_t)), std::ios::cur);

        std::shared_ptr<Entity> entity;
        if (type == EntityType::Hero || type == EntityType::Monster)
        {
            entity = std::make_shared<Character>();
            stream >> *std::static_pointer_cast<Character>(entity);
        }
        else
        {
            entity = std::make_shared<Entity>();
            stream >> *entity;
        }

        room.entities.push_back(entity);
    }

    room.treeCells = KDTree(room.cells);
    return stream;
}

/* *************** General functions *************** */

std::pair<Point, Point> closest_nodes(const Room& room1, const Room& room2)
{
    assert(!room1.getNodes().empty());
    assert(!room2.getNodes().empty());

    int best_dist = std::numeric_limits<int>::max();
    std::pair<int, int> best_cell1({0, 0});
    std::pair<int, int> best_cell2({0, 0});

    for (const Point& cell1 : room1.getNodes())
    {
        for (const Point& cell2 : room2.getNodes())
        {
            Point diff = room1.getPosition() + cell1 - cell2 - room2.getPosition();
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
    assert(!room1.getNodes().empty());
    assert(!room2.getNodes().empty());

    Point cell1, cell2;
    std::tie(cell1, cell2) = closest_nodes(room1, room2);

    std::pair<int, int> diff = room1.getPosition() + cell1 - cell2 - room2.getPosition();
    return std::abs(diff.first) + std::abs(diff.second);
}

void separate_rooms(std::vector<Room>& rooms, int spacing, size_t left, size_t right)
{
    assert(spacing >= 0);
    assert(left < rooms.size() && right <= rooms.size());

    size_t nb_rooms = rooms.size();

    int remaining_iterations = 10 * (right - left); // Maximum number of iterations
    bool go_on = true; // Set to true while we changed something

    while(go_on && remaining_iterations > 0)
    {
        remaining_iterations--;
        go_on = false;
        std::vector<std::pair<int, int>> direction(nb_rooms, {0, 0});

        for (size_t i1 = 0 ; i1 < nb_rooms ; i1++)
        {
            for (size_t i2 = 0 ; i2 < i1 ; i2++)
            {
                // Check wether we are allowed to move them
                bool i1_can_move = left <= i1 && i1 < right;
                bool i2_can_move = left <= i2 && i2 < right;

                if (!i1_can_move && !i2_can_move)
                    break;

                // Takes a distinct pair of rooms.
                Room& room1 = rooms[i1];
                Room& room2 = rooms[i2];

                if (!spaced(room1, room2, spacing))
                {
                    go_on = true;

                    if (i1_can_move && room1.getPosition().first > room2.getPosition().first)
                    {
                        int shift = spacing;//std::max(1.f, (room1.getPosition().first - room2.getPosition().first + spacing) / 6.f);
                        // direction[i1] += {shift, 0};
                        room1.setPosition(room1.getPosition() + std::make_pair(shift, 0));
                    }

                    if (i1_can_move && room1.getPosition().second > room2.getPosition().second)
                    {
                        int shift = spacing;//std::max(1.f, (room1.getPosition().second - room2.getPosition().second + spacing) / 6.f);
                        // direction[i1] += {0, shift};
                        room1.setPosition(room1.getPosition() + std::make_pair(0, shift));
                    }

                    if (i2_can_move && room1.getPosition().first < room2.getPosition().first)
                    {
                        int shift = spacing;//std::max(1.f, (room2.getPosition().first - room1.getPosition().first + spacing) / 6.f);
                        // direction[i2] += {shift, 0};
                        room2.setPosition(room2.getPosition() + std::make_pair(shift, 0));
                    }

                    if (i2_can_move && room1.getPosition().second < room2.getPosition().second)
                    {
                        int shift = spacing;//std::max(1.f, (room2.getPosition().second - room1.getPosition().second + spacing) / 6.f);
                        // direction[i2] += {0, shift};
                        room2.setPosition(room2.getPosition() + std::make_pair(0, shift));
                    }

                    if (room1.getPosition() == room2.getPosition())
                    {
                        // Moves room2 in a random direction.
                        int delta_x = RandGen::uniform_int(-2, 2);
                        int delta_y = RandGen::uniform_int(-2, 2);

                        if (i1_can_move)
                            direction[i1] += {delta_x, delta_y};
                        else if (i2_can_move)
                            direction[i2] -= {delta_x, delta_y};
                    }
                }
            }
        }

        // Apply the position modifiers
        for (size_t i_room = 0 ; i_room < nb_rooms ; i_room++)
            rooms[i_room].setPosition(rooms[i_room].getPosition() + direction[i_room]);
    }
}

void add_monsters(Room& room, float load)
{
    assert(load >= 0.f);
    assert(load <= 100.f);

    // Adds load/100 * size monsters in mean
    size_t nb_monsters = std::floor(room.size() * load / 100.f);
    float excess = (room.size() * load / 100.f) - nb_monsters;
    nb_monsters += (RandGen::uniform_int(0, 1) < excess) ? 1 : 0;

    // Select forbidden cells
    Pattern forbidden;
    for (auto entity : room.getEntities())
        forbidden.insert(std::make_pair(entity->getPosition().x, entity->getPosition().y));

    // Process cells we could place monsters on
    std::vector<std::pair<int, int>> candidates;
    for (const Point& cell : room.getCells())
        if (forbidden.find(cell) == end(forbidden))
            candidates.push_back(cell);
    nb_monsters = std::min(nb_monsters, candidates.size());

    // Select nb_monsters's indexes among all candidates
    std::random_shuffle(begin(candidates), end(candidates));

    for (size_t i_chosen = 0 ; i_chosen < nb_monsters ; i_chosen++)
    {
        // Selected cell of index indexes[i_chosen]
        Point& cell = candidates[i_chosen];
        room.addEntity(std::make_shared<Character>(
            randomClass(),
            sf::Vector2i(cell.first, cell.second)
        ));
    }
}

Room maze_room(int width, int height)
{
    Pattern cells = generate_maze(width, height);
    Room room(cells);

    int max_x = pattern_max_x(room.getCells());
    int max_y = pattern_max_y(room.getCells());

    // Create relevant nodes
    Pattern nodes;

    for (const Point& cell : room.getCells())
        if (cell.first == 0 || cell.second == 0 || cell.first == max_x || cell.second == max_y)
            nodes.insert(cell);

    room.setNodes(nodes);
    return room;
}
