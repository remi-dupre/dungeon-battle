#include <set>
#include <limits>

#include "../src/rand.hpp"
#include "../src/generation/space.hpp"


// Minimum / Maximum values of our integers
constexpr int MIN_VAL = std::numeric_limits<int>::min() / 4;
constexpr int MAX_VAL = std::numeric_limits<int>::max() / 4;

// Number of tests to execute
constexpr int NB_SETS = 100;

// Size of the random set for the test
constexpr int SIZE_SET = 100;


class KDTreeTester : public CxxTest::TestSuite
{
public:
    void testRandSet()
    {
        for (int i_test = 0 ; i_test < NB_SETS ; i_test++)
        {
            // Generate a random set of point
            std::set<Point> set;

            for (int i_point = 0 ; i_point < SIZE_SET ; i_point++)
            {
                set.insert(Point(
                    Rand::uniform_int(MIN_VAL, MAX_VAL),
                    Rand::uniform_int(MIN_VAL, MAX_VAL)
                ));
            }

            KDTree tree(set);

            // Check every points are still in the set
            for (Point point : set)
                TS_ASSERT(tree.closeTo(point, 0));

            // Create new random points, check they are in the set
            for (int i_point = 0 ; i_point < SIZE_SET ; i_point++)
            {
                Point point(
                    Rand::uniform_int(MIN_VAL, MAX_VAL),
                    Rand::uniform_int(MIN_VAL, MAX_VAL)
                );

                TS_ASSERT(
                    set.find(point) != end(set)
                    || !tree.closeTo(point, 0)
                );
            }
        }
    }
};
