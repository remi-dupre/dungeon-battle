#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "args.hpp"
#include "game.hpp"

int main(int argc, char** argv)
{
    std::map<Option, std::string> options;
    options[Option::Config] = Configuration::user_path + "config.ini";

    if (argc > 1)
    {
        int error = parse_arguments(options, argc, argv);
        if (error) // Error while parsing argv
            return error;

        auto help = options.find(Option::Help);
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
