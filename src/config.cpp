#include <fstream>
#include <sstream>
#include <string>

#include "config.hpp"


Configuration::Configuration () :
    width(800),
    height(600),
    fullscreen(false),
    vsync(true)
{}

void Configuration::parse(const std::string& filename)
{
    std::ifstream config_file(filename);
    std::string line;

    while (std::getline(config_file, line))
    {
        std::string::size_type eq_pos = line.find('=');

        if (eq_pos == string::npos)
            continue;

        switch (line.substr(0, eq_pos))
        {
            case "height":
                height = std::stoi(line.substr(eq_pos+1, line.size()));
                break;
            case "width":
                width = std::stoi(line.substr(eq_pos+1, line.size()));
                break;
            case "fullscreen":
                fullscreen = std::stoi(line.substr(eq_pos+1, line.size()));
                break;
            case "vsync":
                vsync = std::stoi(line.substr(eq_pos+1, line.size()));
                break;
            default:
                continue;
        }
    }

    return;
}