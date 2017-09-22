#include <fstream>
#include <string>

#include "config.hpp"


// Default configuration
Configuration::Configuration() :
    width(800),
    height(600),
    scalefactor(1),
    fullscreen(false),
    vsync(true),
    maxfps(60),
    left_key(sf::Keyboard::Key::Q),
    right_key(sf::Keyboard::Key::D),
    up_key(sf::Keyboard::Key::Z),
    down_key(sf::Keyboard::Key::S),
    menu_key(sf::Keyboard::Key::Escape)
{}

void Configuration::read(const std::string& filename)
{
    std::ifstream config_file(filename);
    std::string line;

    if (!config_file.is_open())
        return;

    while (std::getline(config_file, line))
    {
        line = line.substr(0, line.find('#')); // Remove comment

        if (line.length() == 0)
            continue;
        if (line[0] == '[')
            continue;

        std::string::size_type eq_pos = line.find('=');

        if (eq_pos == std::string::npos)
            continue;
        if (eq_pos + 1 > line.size())
            continue;

        std::string option_name = line.substr(0, eq_pos);
        std::string value = line.substr(eq_pos+1, line.size());

        if (option_name == "height")
            height = std::stoi(value);
        else if (option_name == "width")
            width = std::stoi(value);
        else if (option_name == "scalefactor")
            scalefactor = std::stoi(value);
        else if (option_name ==  "fullscreen")
            fullscreen = std::stoi(value);
        else if (option_name ==  "vsync")
            vsync = std::stoi(value);
        else if (option_name ==  "maxfps")
            maxfps = std::stoi(value);
    }

    return;
}

void Configuration::write(const std::string& filename)
{
    std::ofstream config_file(filename);

    if (!config_file.is_open())
        return;

    config_file << "[Video]" << std::endl;
    config_file << "width=" << width << std::endl;
    config_file << "height=" << height << std::endl;
    config_file << "scalefactor=" << scalefactor << std::endl;
    config_file << "fullscreen=" << fullscreen << std::endl;
    config_file << "vsync=" << vsync << std::endl;
    config_file << "maxfps=" << maxfps << std::endl;

    return;
}
