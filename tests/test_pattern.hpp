#include <cxxtest/TestSuite.h>

#include "../src/generation/pattern.hpp"


class PatternTester : public CxxTest::TestSuite
{
public:
    /*
     * Check that bounds are correct for a given pattern
     */
    void testBounds()
    {
        Pattern room = {
            {-2, +1}, {-1, +1}, {+0, +1},
                                {+0, +0},
                                {+0, -1}, {+1, -1}, {+2, -1}
        };

        TS_ASSERT_EQUALS(pattern_min_x(room), -2);
        TS_ASSERT_EQUALS(pattern_max_x(room), +2);

        TS_ASSERT_EQUALS(pattern_min_y(room), -1);
        TS_ASSERT_EQUALS(pattern_max_y(room), +1);
    }

    /*
     * Check trivial cases of superposition.
     */
    void testSuperposition()
    {
        Pattern room = {
                      {+0, -1},
            {-1, +0}, {+0, +0}, {+1, +0},
                      {+0, +1}
        };

        TS_ASSERT(superposed(
            {0, 0}, room,
            {1, 1}, room
        ));
        TS_ASSERT(superposed(
            {2, 0}, room,
            {0, 0}, room
        ));

        TS_ASSERT(!superposed(
            {0, 0}, room,
            {2, 1}, room
        ));
        TS_ASSERT(!superposed(
            {3, 0}, room,
            {0, 0}, room
        ));
    }
};
