#include <cxxtest/TestSuite.h>

#include <map>
#include <string>
#include <random>


#include "../src/generation/pattern.hpp"
#include "../src/generation/pattern.cpp"
#include "../src/generation/room.hpp"
#include "../src/generation/room.cpp"
#include "../src/generation/level.hpp"
#include "../src/generation/level.cpp"
#include "../src/generation/gen_pattern.hpp"
#include "../src/generation/gen_pattern.cpp"
#include "../src/generation/space.hpp"
#include "../src/generation/space.cpp"

#include "../src/map.hpp"
#include "../src/map.cpp"
#include "../src/entity.hpp"
#include "../src/entity.cpp"
#include "../src/rand.hpp"
#include "../src/utility.hpp"
#include "../src/utility.cpp"


// Number of maps to generate for a test
constexpr int NB_MAP_TEST = 100;

constexpr int MIN_ROOMS = 2;
constexpr int MAX_ROOMS = 15;

constexpr int MIN_MARGIN = 0;
constexpr int MAX_MARGIN = 50;

constexpr int ROOM_MIN_SIZE = 50;
constexpr int ROOM_MAX_SIZE = 300;


class GeneratorTester : public CxxTest::TestSuite
{
public:
    /* Test that maps can be generated without any segfault
     */
    void testGeneration()
    {
        GenerationMode gen_options;

        for (int test = 0 ; test < NB_MAP_TEST ; test++)
        {
            TS_TRACE("Generating map " + std::to_string(test+1) + "/" + std::to_string(NB_MAP_TEST));
            gen_options.room_min_size = ROOM_MIN_SIZE;
            gen_options.room_max_size = ROOM_MAX_SIZE;
            gen_options.nb_rooms = Rand::uniform_int(MIN_ROOMS, MAX_ROOMS);
            gen_options.room_margin = Rand::uniform_int(MIN_MARGIN, MAX_MARGIN);
            gen_options.type = static_cast<LevelType>(Rand::uniform_int(0, 1));
            gen_options.monster_load = 3.f;
            gen_options.maze_density = 0.1f;

            TS_TRACE("nb_rooms : " + std::to_string(gen_options.nb_rooms));
            TS_TRACE("room_margin : " + std::to_string(gen_options.room_margin));
            TS_TRACE("type : " + std::to_string(static_cast<int>(gen_options.type)));

            std::random_device r;
            int seed = r();
            Rand::seed(seed);
            TS_TRACE("Seed : " + std::to_string(seed));

            Level level = generate(gen_options);
            Map map = std::move(level.map);
            auto entities = std::move(level.entities);


            for (auto& entity : entities)
            {
                // Check that all the entitites are placed on a floor
                TS_ASSERT(
                    map.cellAt(entity->getPosition().x, entity->getPosition().y) == CellType::Floor
                );

                // Check that 2 entities are not stacked (for now, none can)
                for (auto& entity2 : entities)
                    TS_ASSERT(
                           entity == entity2
                        || entity->getPosition() != entity2->getPosition()
                    );
            }
        }
    }
};
