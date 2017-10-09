#include "room.hpp"


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
    float ratio = Random::uniform_float((2.f / 3.f), (4.f / 3.f));

    int width = std::max(2, static_cast<int>(ratio * medium_width));
    int height = std::max(2, size / width);
    return generate_rectangle(width, height);
}

Pattern generate_hallway(std::pair<int, int> cell1, std::pair<int, int> cell2)
{
    int x1, y1, x2, y2;
    std::tie(x1, y1) = cell1;
    std::tie(x2, y2) = cell2;

    int x=0, y=0;
    Pattern path;
    path.insert({0, 0});

    while (x1 + x != x2 || y1 + y != y2)
    {
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

        std::advance(selected, Random::uniform_int(0, surrounding.size()-1));
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
    int x1 = Random::uniform_int(-avg_side/2, avg_side/2);
    int y1 = Random::uniform_int(-avg_side/2, avg_side/2);
    int x2 = Random::uniform_int(-avg_side/2, avg_side/2);
    int y2 = Random::uniform_int(-avg_side/2, avg_side/2);

    auto path1 = generate_hallway({0, 0}, {x1, y1});
    auto path2 = generate_hallway({0, 0}, {x2, y2});

    return merged_patterns({{0, 0}, {0, 0}}, {path1, path2});
}


void add_monsters(const Pattern& room, std::vector<std::shared_ptr<Entity>>& entities, float load)
{
    assert(load >= 0.f);
    assert(load <= 100.f);

    unsigned int nb_monsters = room.size() * load / 100.f;

    std::set<int> chosen_cells; // Indexes of cells on which we place monsters
    while (chosen_cells.size() < nb_monsters)
        chosen_cells.insert(Random::uniform_int(0, room.size()-1));

    int index = 0;
    for (auto& cell : room)
    {
        if (chosen_cells.find(index) != end(chosen_cells))
        {
            entities.push_back(std::make_shared<Character>(
                EntityType::Monster,
                Interaction::None,
                sf::Vector2i(cell.first, cell.second),
                Direction::Left, 3, 1
            ));
        }
        index++;
    }
}


std::vector<std::shared_ptr<Entity>> merged_entities(
   const std::vector<std::pair<int, int>>& positions,
   std::vector<std::vector<std::shared_ptr<Entity>>>& entities)
{
    assert(positions.size() == entities.size());

    for (size_t i_room = 0 ; i_room < positions.size() ; i_room++)
    {
        for (auto& entity : entities[i_room])
        {
            entity->setPosition(sf::Vector2i(
                entity->getPosition().x + positions[i_room].first,
                entity->getPosition().y + positions[i_room].second
            ));
        }
    }

    std::vector<std::shared_ptr<Entity>> merged_entities;
    for (auto& room_entities : entities)
        merged_entities.insert(std::end(merged_entities), std::begin(room_entities), std::end(room_entities));

    return merged_entities;
}
