#include <set>
#include <limits>

#include "../src/rand.hpp"
#include "../src/generation/space.hpp"


// Minimum / Maximum values of our integers
constexpr int MIN_VAL = -100;
constexpr int MAX_VAL = 100;

// Number of tests to execute
constexpr int NB_SETS = 100;

// Size of the random set for the test
constexpr int SIZE_SET = 1000;


class KDTreeTester : public CxxTest::TestSuite
{
public:
    /*
     * Test inclusion of random points in a random set.
     */
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

            // Create new random points, check they are not in the set
            for (int i_point = 0 ; i_point < SIZE_SET ; i_point++)
            {
                Point point(
                    Rand::uniform_int(MIN_VAL, MAX_VAL),
                    Rand::uniform_int(MIN_VAL, MAX_VAL)
                );

                TS_ASSERT(
                    !tree.closeTo(point, 0)
                     || set.find(point) != end(set)
                );
            }
        }
    }

    /*
     * Test distance of a point to a set containing only {0, 0}.
     */
    void testSingleton()
    {
        KDTree tree({Point(0, 0)});
        TS_ASSERT( tree.closeTo(Point(0, 0), 0) );

        TS_ASSERT( tree.closeTo(Point(10, 0), 10) );
        TS_ASSERT( !tree.closeTo(Point(10, 0), 9) );

        TS_ASSERT( tree.closeTo(Point(0, 10), 10) );
        TS_ASSERT( !tree.closeTo(Point(0, 10), 9) );
    }

    /*
     * Test distance of a point to a set containing only {0, 0}.
     */
    void testStar()
    {
        /*
         * ...X...
         * ..X.X..
         * ...X...
         */
        KDTree tree({
            Point(1, 0), Point(-1, 0),
            Point(0, 1), Point(0, -1)
        });

        TS_ASSERT( tree.closeTo(Point(0, 0), 1) );
        TS_ASSERT( !tree.closeTo(Point(0, 0), 0) );

        TS_ASSERT( tree.closeTo(Point(3, -3), 5) );
        TS_ASSERT( !tree.closeTo(Point(3, -3), 4) );
    }
};
