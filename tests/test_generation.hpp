#include <cxxtest/TestSuite.h>

#include <iterator>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "../src/generation/generator.hpp"

#include "../src/map.hpp"
#include "../src/entity.hpp"
#include "../src/rand.hpp"
#include "../src/utility.hpp"


// Number of maps to generate for a test
constexpr int NB_MAP_TEST = 10;
constexpr int NB_CHUNK = 10;

// Density of rooms we generate
constexpr int MIN_ROOMS = 2;
constexpr int MAX_ROOMS = 15;

// Spacement of the rooms
constexpr int MIN_MARGIN = 0;
constexpr int MAX_MARGIN = 50;

// Size of the rooms
constexpr int ROOM_MIN_SIZE = 50;
constexpr int ROOM_MAX_SIZE = 300;


class GeneratorTester : public CxxTest::TestSuite
{
public:
    /* Test that maps can be generated without any segfault
     */
    void testFiniteGeneration()
    {
        GenerationMode gen_options;

        for (int test = 0 ; test < NB_MAP_TEST ; test++)
        {
            TS_TRACE("Generating map " + std::to_string(test+1) + "/" + std::to_string(NB_MAP_TEST) + (" (finite)"));

            gen_options.room_min_size = ROOM_MIN_SIZE;
            gen_options.room_max_size = ROOM_MAX_SIZE;
            gen_options.nb_rooms = Rand::uniform_int(MIN_ROOMS, MAX_ROOMS);
            gen_options.room_margin = Rand::uniform_int(MIN_MARGIN, MAX_MARGIN);
            gen_options.type = static_cast<LevelType>(Rand::uniform_int(0, 1));
            gen_options.monster_load = 3.f;
            gen_options.maze_density = 0.1f;
            gen_options.infinite = false;

            TS_TRACE("nb_rooms : " + std::to_string(gen_options.nb_rooms));
            TS_TRACE("room_margin : " + std::to_string(gen_options.room_margin));
            TS_TRACE("type : " + std::to_string(static_cast<int>(gen_options.type)));

            std::random_device r;
            int seed = r();
            RandGen::seed(seed);
            TS_TRACE("Seed : " + std::to_string(seed));

            Generator generator(gen_options);
            Map map;
            std::vector<std::shared_ptr<Entity>> entities;

            // Get a squared section of chunks
            for (int x = 0 ; x < NB_CHUNK ; x++)
            {
                for (int y = 0 ; y < NB_CHUNK ; y++)
                {
                    map.setChunk(x, y, generator.getChunkCells(x, y));

                    auto new_entities = generator.getChunkEntities(x, y);
                    entities.insert(end(entities), begin(new_entities), end(new_entities));
                }
            }

            for (auto& entity : entities)
            {
                // Check that all the entitites are placed on a floor
                TS_ASSERT(
                    map.cellAt(entity->getPosition().x, entity->getPosition().y) == CellType::Floor
                );

                // Check that 2 entities are not stacked (for now, none can)
                for (auto& entity2 : entities)
                {
                    TS_ASSERT(
                           entity == entity2
                        || entity->getPosition() != entity2->getPosition()
                    );
                }
            }
        }
    }
};
