#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "args.hpp"
#include "game.hpp"


int main(int argc, char** argv)
{
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

    Game game;

    game.init(options);

    game.run();

    return 0;
}
