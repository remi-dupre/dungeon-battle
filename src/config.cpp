#include <fstream>
#include <sstream>
#include <string>

#include "config.hpp"


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

    while (std::getline(config_file, line))
    {
        if (line[0] == '#' || line[0] == '[')
            continue;

        std::string::size_type eq_pos = line.find('=');

        if (eq_pos == std::string::npos)
            continue;

        std::string option_name = line.substr(0, eq_pos);

        if (option_name == "height")
            height = std::stoi(line.substr(eq_pos+1, line.size()));
        else if (option_name == "width")
            width = std::stoi(line.substr(eq_pos+1, line.size()));
        else if (option_name ==  "fullscreen")
            fullscreen = std::stoi(line.substr(eq_pos+1, line.size()));
        else if (option_name ==  "vsync")
            vsync = std::stoi(line.substr(eq_pos+1, line.size()));
    }

    return;
}

void Configuration::write(const std::string& filename)
{
    std::ofstream config_file(filename);
    
    config_file << "[Video]" << std::endl;
    config_file << "width=" << width << std::endl;
    config_file << "height=" << height << std::endl;
    config_file << "fullscreen=" << fullscreen << std::endl;
    config_file << "vsync=" << vsync << std::endl;
}
