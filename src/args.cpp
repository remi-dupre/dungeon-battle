#include "args.hpp"


// (option string, option action, reads a name)
constexpr std::tuple<const char*, Option, bool> command_line_options[3] =
{
    std::make_tuple("c", Option::Config, true),
    std::make_tuple("config", Option::Config, true),
    std::make_tuple("help", Option::Help, false)
};


int parse_arguments(std::map<Option, std::string>& options, int argc, char** argv)
{
    for (int n = 1; n < argc; n++)
    {
        bool read_name = false; // Read the next argument as a name passed to the current argument
        Option option; // Current option

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
                const char* p = std::strchr(&argv[n][i], '=');
                std::size_t j;

                if (p != nullptr)
                {
                    std::size_t len = static_cast<std::size_t>(p - &argv[n][i]);

                    for (j = 0; j < array_length(command_line_options); j++)
                    {
                        if (std::strlen(std::get<const char*>(command_line_options[j])) == len)
                        {
                            if (std::memcmp(&argv[n][i], std::get<const char*>(command_line_options[j]), len) == 0)
                            {
                                option = std::get<Option>(command_line_options[j]);
                                read_name = false;
                                break;
                            }
                        }
                    }

                    if (j != array_length(command_line_options))
                        options[option] = &argv[n][len+i+1];
                }
                else
                {
                    for (j = 0; j < array_length(command_line_options); j++)
                    {
                        if (std::strcmp(&argv[n][i], std::get<const char*>(command_line_options[j])) == 0)
                        {
                            option = std::get<Option>(command_line_options[j]);
                            read_name = std::get<bool>(command_line_options[j]);
                            break;
                        }
                    }
                }

                if (j == array_length(command_line_options))
                {
                    std::cerr << "Unknown option: " << argv[n] << "." << std::endl;
                }

                break;
            }
        }

        if (read_name)
        {
            if (n == argc - 1)
                return 1; // A name is missing

            options[option] = std::string(argv[++n]);
        }
    }

    return 0;
}
