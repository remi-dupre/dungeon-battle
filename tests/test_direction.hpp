#include "../src/utility.hpp"

class DirectionTest : public CxxTest::TestSuite
{
public:
    void testDirection()
    {
        Direction dir1 = Direction::None, dir2 = Direction::None;

        for (auto dir : directions)
            TS_ASSERT(!has_direction(dir1, dir));

        dir1 |= Direction::Up | Direction::Left;

        TS_ASSERT(has_direction(dir1, Direction::Up));
        TS_ASSERT(has_direction(dir1, Direction::Left));
        TS_ASSERT(!has_direction(dir1, Direction::Right));
        TS_ASSERT(!has_direction(dir1, Direction::Down));

        dir2 = dir1 & Direction::Left;

        TS_ASSERT(!has_direction(dir2, Direction::Up));
        TS_ASSERT(has_direction(dir2, Direction::Left));
        TS_ASSERT(!has_direction(dir2, Direction::Right));
        TS_ASSERT(!has_direction(dir2, Direction::Down));

        TS_ASSERT(to_vector2i(Direction::Right) == sf::Vector2i(1, 0));
        TS_ASSERT(to_vector2i(Direction::Down) == sf::Vector2i(0, 1));
        TS_ASSERT(to_vector2i(Direction::Up) == sf::Vector2i(0, -1));
        TS_ASSERT(to_vector2i(Direction::Left | Direction::Right) == sf::Vector2i(0, 0));
        TS_ASSERT(to_vector2i(Direction::Up | Direction::Right | Direction::Left) == sf::Vector2i(0, -1));

    }
};
