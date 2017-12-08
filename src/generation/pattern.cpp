#include "pattern.hpp"


int pattern_min_x(const Pattern& pattern)
{
    return std::min_element(
        begin(pattern), end(pattern),
        [](const Point& a, const Point& b) {
            return a.first < b.first;
        }
    )->first;
}

int pattern_max_x(const Pattern& pattern)
{
    return std::max_element(
        begin(pattern), end(pattern),
        [](const Point& a, const Point& b) {
            return a.first < b.first;
        }
    )->first;
}

int pattern_min_y(const Pattern& pattern)
{
    return std::min_element(
        begin(pattern), end(pattern),
        [](const Point& a, const Point& b) {
            return a.second < b.second;
        }
    )->second;
}

int pattern_max_y(const Pattern& pattern)
{
    return std::max_element(
        begin(pattern), end(pattern),
        [](const Point& a, const Point& b) {
            return a.second < b.second;
        }
    )->second;
}


Pattern frontier(const Pattern& pattern)
{
    Pattern frontier;
    for (const Point& cell : pattern)
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

Pattern surrounding(const Pattern& pattern)
{
    Pattern surrounding;

    for (const Point& cell : pattern)
    {
        Point relat;

        for (relat.first = -1 ; relat.first <= 1 ; relat.first++)
        {
            for (relat.second = -1 ; relat.second <= 1 ; relat.second ++)
            {
                if (pattern.find(cell + relat) == end(pattern))
                    surrounding.insert(cell + relat);
            }
        }
    }

    return surrounding;
}

bool spaced(Point position1, const Pattern& pattern1, Point position2, const Pattern& pattern2, int spacing)
{
    for (const Point& cell1 : pattern1)
    {
        for (const Point& cell2 : pattern2)
        {
            // Calculate real coordinates of both points
            Point point1 = cell1 + position1;
            Point point2 = cell2 + position2;
            // Check the distance between these points
            if (std::abs(point1.first - point2.first) + std::abs(point1.second - point2.second) <= spacing)
            {
                return false;
            }
        }
    }
    return true;
}

bool superposed(Point position1, const Pattern& pattern1, Point position2, const Pattern& pattern2)
{
    // The complexity would be pattern1.size() * log(pattern2.size())
    if(pattern1.size() > pattern2.size())
        return superposed(position2, pattern2, position1, pattern1);

    for (const Point& cell : pattern1)
    {
        // Must check for all points P1 of pattern1, P2 of pattern2 :
        //   position1 + P1 != position2 + P2
        // pos_in_2 represents the theorical coordinate of a point of pattern1 in pattern2 :
        Point pos_in_2 = position1 + cell - position2;
        if (pattern2.find(pos_in_2) != end(pattern2))
            return true;
    }
    return false;
}

Pattern merged_patterns(const std::vector<Point>& positions, const std::vector<Pattern>& patterns)
{
    // Every pattern must have a position
    assert(positions.size() == patterns.size());

    // Contains the new postions of every cells
    std::set<Point> fullMap;

    for (size_t i_pattern = 0 ; i_pattern < patterns.size() ; i_pattern++)
        for(const Point& pattern_cell : patterns[i_pattern])
            fullMap.insert(positions[i_pattern] + pattern_cell);

    return fullMap;
}
