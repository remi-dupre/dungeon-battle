/**
 * \file space.hpp
 * \brief Space representation algorithms and structures.
 */

#pragma once

#include <algorithm>
#include <memory>
#include <set>
#include <tuple>
#include <vector>


/**
 * \brief Represent an arbitrary point in a 2D dimension space.
 */
typedef std::pair<int, int> Point;

/**
 * Gives the distance between two points.
 */
int distance(Point a, Point b);


/**
 * \brief Partition a set of point as a kd-tree.
 */
class KDTree
{
public:
    /**
     * \brief Construct a kd-tree representing an array of points.
     * \param points The set containing all points we will add
     */
    KDTree(const std::set<Point>& points);

    /**
     * \brief Check if a point is closer than a given range.
     * \param point A point of the space.
     * \param space The maximal distance the point should be to this set.
     * \return true if there is a point of the set closer than 'distance'.
     */
    bool closeTo(Point point, int space) const;

private:
    /**
     * \brief Construct a kd-tree representing a subarray of points.
     * \param begin The begining of the subarray we add in the tree.
     * \param end The end (excluded) of the subarray we add in the tree.
     */
    KDTree(std::vector<Point>::iterator begin, std::vector<Point>::iterator end, bool vertically);

    bool vertical_cut; ///< Wether the cut is vertical or horizontal.

    std::unique_ptr<Point> center; ///< The point represented by the node, space is cut with a line passing through this point.

    /**
     * \brief The two subtrees.
     * If the cut is vertical, the first one is on the left of *center, and the second one on its right.
     * If the cut is horizontal, the first one is on the top of *center, and the second one on its bottom.
     */
    std::unique_ptr<std::pair<KDTree, KDTree>> childs; ///< The two subtrees.
};
