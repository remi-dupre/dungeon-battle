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

#include "../src/config.hpp"
#include "../src/config.cpp"
#include "../src/args.hpp"
#include "../src/args.cpp"
#include "../src/map.hpp"
#include "../src/map.cpp"
#include "../src/entity.hpp"
#include "../src/entity.cpp"
#include "../src/rand.hpp"
#include "../src/rand.cpp"
#include "../src/utility.hpp"
#include "../src/utility.cpp"


#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wwritable-strings"


class ConfigTester : public CxxTest::TestSuite
{
public:
    void testFile1()
    {
        Configuration config;
        config.read("tests/config1.ini");

        TS_ASSERT_EQUALS(config.width, 800);
        TS_ASSERT_EQUALS(config.height, 600);
        TS_ASSERT_EQUALS(config.scalefactor, 1);
        TS_ASSERT_EQUALS(config.fullscreen, false);
        TS_ASSERT_EQUALS(config.vsync, true);
        TS_ASSERT_EQUALS(config.maxfps, 60);
    }

    void testFile2()
    {
        Configuration config;
        config.read("tests/config2.ini");

        TS_ASSERT_EQUALS(config.width, 685);
        TS_ASSERT_EQUALS(config.height, 534);
        TS_ASSERT_EQUALS(config.scalefactor, 2);
        TS_ASSERT_EQUALS(config.fullscreen, true);
        TS_ASSERT_EQUALS(config.vsync, true);
        TS_ASSERT_EQUALS(config.maxfps, 120);
    }
};


class ParserTester : public CxxTest::TestSuite
{
public:
    void testArgs1()
    {
        std::map<Option, std::string> options;

        char *argv[] =
        {
            "",
            "--config",
            "config3.ini"
        };

        TS_ASSERT_EQUALS(parse_arguments(options, array_length(argv), argv), 0);
        TS_ASSERT_EQUALS(options[Option::Config], "config3.ini");
    }

    void testArgs2()
    {
        std::map<Option, std::string> options;

        char *argv[] =
        {
            "",
            "--config=config2.ini"
        };

        TS_ASSERT_EQUALS(parse_arguments(options, array_length(argv), argv), 0);
        TS_ASSERT_EQUALS(options[Option::Config], "config2.ini");
    }
};


// Number of maps to generate for a test
constexpr int NB_MAP_TEST = 50;

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
            gen_options.nb_rooms = Random::uniform_int(MIN_ROOMS, MAX_ROOMS);
            gen_options.room_margin = Random::uniform_int(MIN_MARGIN, MAX_MARGIN);
            gen_options.type = static_cast<LevelType>(Random::uniform_int(0, 1));

            TS_TRACE("nb_rooms : " + std::to_string(gen_options.nb_rooms));
            TS_TRACE("room_margin : " + std::to_string(gen_options.room_margin));
            TS_TRACE("type : " + std::to_string(static_cast<int>(gen_options.type)));

            std::random_device r;
            int seed = r();
            Random::seed(seed);
            TS_TRACE("Seed : " + std::to_string(seed));

            auto level = generate(gen_options);
            auto& map = std::get<0>(level);
            auto& entities = std::get<1>(level);

            // Check that the stairs are placed on a floor
            for (auto& entity : entities)
                TS_ASSERT(
                    entity->getType() != EntityType::Stairs
                    || map.cellAt(entity->getPosition().x, entity->getPosition().y) == CellType::Floor
                );
        }
    }
};
