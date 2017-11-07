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

#include "../src/config.hpp"
#include "../src/config.cpp"
#include "../src/args.hpp"
#include "../src/args.cpp"
#include "../src/map.hpp"
#include "../src/map.cpp"
#include "../src/entity.hpp"
#include "../src/entity.cpp"
#include "../src/rand.hpp"
#include "../src/utility.hpp"
#include "../src/utility.cpp"


#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wwritable-strings"


// Number of maps to generate for a test
constexpr int NB_GAME_TEST = 5;

constexpr int MIN_ROOMS = 2;
constexpr int MAX_ROOMS = 15;

constexpr int MIN_MARGIN = 0;
constexpr int MAX_MARGIN = 50;

constexpr int ROOM_MIN_SIZE = 50;
constexpr int ROOM_MAX_SIZE = 300;

constexpr int MIN_TURNS = 1000;
constexpr int MAX_TURNS = 3000;

Action randomaction(){
std::vector<Action> allaction;
   allaction.push_back(Action(ActionType::Attack, Direction::Left));
   allaction.push_back(Action(ActionType::Attack, Direction::Right));
   allaction.push_back(Action(ActionType::Attack, Direction::Up));
   allaction.push_back(Action(ActionType::Attack, Direction::Down));
   allaction.push_back(Action(ActionType::Move, Direction::Left));
   allaction.push_back(Action(ActionType::Move, Direction::Right));
   allaction.push_back(Action(ActionType::Move, Direction::Up));
   allaction.push_back(Action(ActionType::Move, Direction::Down));
   allaction.push_back(Action(ActionType::Interact);
   allaction.push_back(Action());
   return allaction[Rand::uniform_int(0,9)];
}

class GameTester : public CxxTest::TestSuite
{
public:
    /* Test that maps can be generated without any segfault
     */
    void testGeneration()
    {
        GenerationMode gen_options;

        for (int test = 0 ; test < NB_GAME_TEST ; test++)
        {
            /*TS_TRACE("Generating map " + std::to_string(test+1) + "/" + std::to_string(NB_MAP_TEST));
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
            Map map = std::move(level.map);*/

            std::map<Option, std::string> options;
            options[Option::Config] = "config.ini";

            Game game;
            game.init(options.insert({Option::NoPlayer , ""}));
            game.run();

        }
    }
};
