#include <fstream>
#include <sstream>
#include <string>

#include "config.hpp"

// Default configuration
Configuration::Configuration() :
    width(800),
    height(600),
    fullscreen(false),
    vsync(true)
{}

void Configuration::parse(const std::string& filename)
{
    std::ifstream config_file(filename);
    std::string line;

    if (!config_file.is_open())
	return;

    while (std::getline(config_file, line))
    {
        if (line[0] == '[')
            continue;
	line = line.substr(0, line.find('#')); // Remove commentary at the end of line

        std::string::size_type eq_pos = line.find('=');

        if (eq_pos == std::string::npos)
            continue;
	if (eq_pos + 1 >= line.size())
	    continue;

        std::string option_name = line.substr(0, eq_pos);
	std::string value = line.substr(eq_pos+1, line.size());

        if (option_name == "height")
            height = std::stoi(value);
        else if (option_name == "width")
            width = std::stoi(value);
        else if (option_name ==  "fullscreen")
            fullscreen = std::stoi(value);
        else if (option_name ==  "vsync")
            vsync = std::stoi(value);
    }

    return;
}
