#include <map>
#include <string>
#include <ctime>
#include <random>

#include <SFML/Graphics.hpp>

#include "args.hpp"
#include "game.hpp"

#include "generation.hpp"


int main(int argc, char** argv)
{
    std::srand(std::time(nullptr));

    std::map<Option, std::string> options;
    options[Option::Config] = "config.ini";

    if (argc > 1)
    {
        int error = parse_arguments(options, argc, argv);

        auto help = options.find(Option::Help);

        if (error) // Error while parsing argv
            return error;

        if(help != options.end())
        {
            // TODO: Display help message
            return 0;
        }
    }

    // Generate a map for a console demo
    GenerationMode gen_options({
        .width = 100,
        .height = 40,
        .nb_rooms = 2,
        .room_size = 650
    });
    auto level = generate(gen_options);
    auto map = std::get<Map>(level);
    std::cout << map;

    Game game;
    game.init(options);
    game.run();

    return 0;
}
