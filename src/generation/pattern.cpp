#include "pattern.hpp"


int pattern_min_x(const Pattern& pattern)
{
    return std::min_element(
        begin(pattern), end(pattern),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        }
    )->first;
}

int pattern_max_x(const Pattern& pattern)
{
    return std::max_element(
        begin(pattern), end(pattern),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        }
    )->first;
}

int pattern_min_y(const Pattern& pattern)
{
    return std::min_element(
        begin(pattern), end(pattern),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }
    )->second;
}

int pattern_max_y(const Pattern& pattern)
{
    return std::max_element(
        begin(pattern), end(pattern),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }
    )->second;
}


Pattern surrounding(const Pattern& pattern)
{
    Pattern frontier;
    for (const auto& cell : pattern)
    {
        bool outside =
            pattern.find(cell + std::make_pair(1, 0)) == end(pattern)
         || pattern.find(cell - std::make_pair(1, 0)) == end(pattern)
         || pattern.find(cell + std::make_pair(0, 1)) == end(pattern)
         || pattern.find(cell - std::make_pair(0, 1)) == end(pattern);

        if (outside)
            frontier.insert(cell);
    }
    return frontier;
}


bool spaced(
    std::pair<int, int> position1, const Pattern& pattern1,
    std::pair<int, int> position2, const Pattern& pattern2,
    int spacing)
{
    for (auto& cell1 : pattern1)
    {
        for (auto& cell2 : pattern2)
        {
            // Calculate real coordinates of both points
            std::pair<int, int> point1 = cell1 + position1;
            std::pair<int, int> point2 = cell2 + position2;
            // Check the distance between these points
            if (std::abs(point1.first - point2.first) + std::abs(point1.second - point2.second) <= spacing)
            {
                return false;
            }
        }
    }
    return true;
}

bool superposed(
    std::pair<int, int> position1, const Pattern& pattern1,
    std::pair<int, int> position2, const Pattern& pattern2)
{
    for (auto& cell : pattern1)
    {
        // Must check for all points P1 of pattern1, P2 of pattern2 :
        //   position1 + P1 != position2 + P2
        // pos_in_2 represents the theorical coordinate of a point of pattern1 in pattern2 :
        std::pair<int, int> pos_in_2 = position1 + cell - position2;
        if (pattern2.find(pos_in_2) != end(pattern2))
            return true;
    }
    return false;
}


Pattern normalized_pattern(const Pattern& pattern, std::vector<std::shared_ptr<Entity>>& entities)
{
    // Removes 1 to add a margin
    int min_x = pattern_min_x(pattern) - 1;
    int min_y = pattern_min_y(pattern) - 1;

    Pattern normalized;

    for (auto& cell : pattern)
    {
        assert(cell.first  - min_x >= 0);
        assert(cell.second - min_y >= 0);
        normalized.insert({cell.first - min_x, cell.second - min_y});
    }

    for (auto& entity : entities)
    {
        auto pos = entity->getPosition();
        entity->setPosition({pos.x - min_x, pos.y - min_y});
    }

    return normalized;
}

Pattern merged_patterns(
    const std::vector<std::pair<int, int>>& positions,
    const std::vector<Pattern>& patterns)
{
    // Every pattern must have a position
    assert(positions.size() == patterns.size());

    // Contains the new postions of every cells
    std::set<std::pair<int, int>> fullMap;

    for (size_t i_pattern = 0 ; i_pattern < patterns.size() ; i_pattern++)
        for(auto pattern_cell : patterns[i_pattern])
            fullMap.insert(positions[i_pattern] + pattern_cell);

    return fullMap;
}
