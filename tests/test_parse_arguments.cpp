#include <cassert>
#include <iostream>
#include <map>
#include <string>

#include "../src/args.hpp"
#include "../src/args.cpp"

#pragma GCC diagnostic ignored "-Wwritable-strings"


int main()
{
    {
        std::map<Option, std::string> options;

        char *argv[] =
        {
            "",
            "--config",
            "config3.ini"
        };

        assert(parse_arguments(options, 3, argv) == 0);

        assert(options[Option::Config] == "config3.ini");
    }

    {
        std::map<Option, std::string> options;

        char *argv[] =
        {
            "",
            "--config=config2.ini"
        };

        assert(parse_arguments(options, 2, argv) == 0);

        assert(options[Option::Config] == "config2.ini");
    }

    std::cout << "Argument parser OK" << std::endl;

    return 0;
}
