#include <cstring>
#include <iostream>
#include <map>
#include <string>

#include "args.hpp"


template <typename T, int N>
inline constexpr std::size_t array_length(T (&)[N])
{
    return N;
}

struct NameOption
{
    const char *name;
    Option option;
};

inline constexpr NameOption command_line_options[3] =
{
    { "c", Option::Config },
    { "config", Option::Config },
    { "help", Option::Help }
};


int parse_arguments(std::map<Option, std::string>& options, int argc, char *argv[])
{
    bool read_name = false; // Read the next argument as a name passed to the current argument
    Option option; // Current option

    for (int n = 1; n < argc; n++)
    {
        for (std::size_t i = 0; argv[n][i] != '\0'; i++)
        {
            if (i == 0 && argv[n][0] == '-')
                continue;
            else if (i == 0)
                return 1;
            else if (i == 1 && argv[n][1] == '-')
            {
                if (argv[n][2] == '\0')
                    return 0;

                continue;
            }
            else
            {
                for (std::size_t j = 0; j < array_length(command_line_options); j++)
                {
                    if (std::strcmp(&argv[n][i], command_line_options[j].name) == 0)
                    {
                        switch (option = command_line_options[j].option)
                        {
                            case Option::Config:
                                read_name = true;
                                break;
                            default:
                                return 1;
                                break;
                        }
                    }
                    else
                    {
                        std::cout << "Unknown option: " << argv[n][i] << ".";
                    }
                }
            }
        }

        if (read_name)
        {
            if (n == argc - 1)
                return 1; // A name is missing

            options[option] = std::string(argv[++n]);
        }
    }

    auto help = options.find(Option::Help);

    if(help != options.end())
    {
        // Display help message

        return -1;
    }

    return 0;
}
