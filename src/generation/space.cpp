#include "space.hpp"


int distance(Point a, Point b)
{
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

KDTree::KDTree(const std::set<Point>& points)
{
    std::vector<Point> copy;
    std::copy(std::begin(points), std::end(points), std::back_inserter(copy));
    *this = KDTree(std::begin(copy), std::end(copy), true);
}

KDTree::KDTree(std::vector<Point>::iterator begin, std::vector<Point>::iterator end, bool vertically) :
    vertical_cut(vertically)
{
    if (begin == end)
    {
        // Create an empty tree
        center = nullptr;
        childs = nullptr;
    }
    else
    {
        // Select the median point and create the two corresponding subtrees
        std::vector<Point>::iterator mid = begin + ((end - begin) / 2);
        std::nth_element(begin, mid, end, [this](Point& a, Point& b) {
            if (vertical_cut)
                return a.first < b.first;
            else
                return a.second < b.second;
        });

        center = std::make_unique<Point>(*mid);
        childs = std::make_unique<std::pair<KDTree, KDTree>>(std::make_pair(
            KDTree(begin, mid, !vertically),
            KDTree(mid+1, end, !vertically)
        ));
    }
}

bool KDTree::closeTo(Point point, int space) const
{
    if (center == nullptr)
        return false;
    if (distance(*center, point) <= space)
        return true;

    // Project the points on the axis we cutted.
    int point_pos, center_pos;
    if (vertical_cut)
    {
        point_pos = point.first;
        center_pos = center->first;
    }
    else
    {
        point_pos = point.second;
        center_pos = center->second;
    }

    // Check if there is a trivial deduction to make.
    if (std::abs(point_pos - center_pos) > space)
    {
        if (point_pos < center_pos)
            return childs->first.closeTo(point, space);
        else
            return childs->second.closeTo(point, space);
    }
    else
    {
        if (point_pos < center_pos)
            return childs->first.closeTo(point, space) || childs->second.closeTo(point, space);
        else
            return childs->second.closeTo(point, space) || childs->first.closeTo(point, space);
    }
}
