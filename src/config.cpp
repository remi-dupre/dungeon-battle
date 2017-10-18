#include <fstream>
#include <stdexcept>

#include "config.hpp"


// Default configuration
Configuration::Configuration() :
    width(800),
    height(600),
    scalefactor(1),
    fullscreen(false),
    vsync(true),
    maxfps(60),
    animation_speed(3.f),
    left_key(sf::Keyboard::Key::Q),
    right_key(sf::Keyboard::Key::D),
    up_key(sf::Keyboard::Key::Z),
    down_key(sf::Keyboard::Key::S),
    menu_key(sf::Keyboard::Key::Escape),
    interaction_key(sf::Keyboard::Key::E),
    attack_left_key(sf::Keyboard::Key::Left),
    attack_right_key(sf::Keyboard::Key::Right),
    attack_up_key(sf::Keyboard::Key::Up),
    attack_down_key(sf::Keyboard::Key::Down),
    gen_options()
{}

sf::Keyboard::Key string_to_key(const std::string& name)
{
    if (name.length() == 1)
    {
        if ('A' <= name[0] && name[0] <= 'Z')
        {
            return static_cast<sf::Keyboard::Key>(static_cast<char>(sf::Keyboard::Key::A) + name[0] - 'A');
        }
        else if ('a' <= name[0] && name[0] <= 'z')
        {
            return static_cast<sf::Keyboard::Key>(static_cast<char>(sf::Keyboard::Key::A) + name[0] - 'a');
        }
    }
    else if (name == "Left" || name == "left")
    {
        return sf::Keyboard::Key::Left;
    }
    else if (name == "Right" || name == "right")
    {
        return sf::Keyboard::Key::Right;
    }
    else if (name == "Up" || name == "up")
    {
        return sf::Keyboard::Key::Up;
    }
    else if (name == "Down" || name == "down")
    {
        return sf::Keyboard::Key::Down;
    }
    else if (name == "Escape" || name == "escape")
    {
        return sf::Keyboard::Key::Escape;
    }

    return {};
}

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

        try
        {
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
            else if (option_name ==  "animation_speed")
                animation_speed = std::stof(value);
        }
        catch (const std::invalid_argument& e)
        {
            continue;
        }
        catch (const std::out_of_range& e)
        {
            continue;
        }
    }

    return;
}

void Configuration::readGame(const std::string& filename)
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

        try
        {
            if (option_name == "room_min_size")
                gen_options.room_min_size = std::stoi(value);
            else if (option_name == "room_max_size")
                gen_options.room_max_size = std::stoi(value);
            else if (option_name == "nb_rooms")
                gen_options.nb_rooms = std::stoi(value);
            else if (option_name ==  "room_margin")
                gen_options.room_margin = std::stoi(value);
            else if (option_name ==  "monster_load")
                gen_options.monster_load = std::stoi(value);
            else if (option_name ==  "maze_density")
                gen_options.maze_density = std::stoi(value);
            else if (option_name ==  "generation_type")
                gen_options.type = static_cast<LevelType>(std::stoi(value));
        }
        catch (const std::invalid_argument& e)
        {
            continue;
        }
        catch (const std::out_of_range& e)
        {
            continue;
        }
    }

    return;
}

std::ostream& operator<<(std::ostream& stream, const Configuration& config)
{
    return stream <<
        "[Video]" << std::endl <<
        "width=" << config.width << std::endl <<
        "height=" << config.height << std::endl <<
        "scalefactor=" << config.scalefactor << std::endl <<
        "fullscreen=" << config.fullscreen << std::endl <<
        "vsync=" << config.vsync << std::endl <<
        "maxfps=" << config.maxfps << std::endl <<
        "animation_speed=" << config.animation_speed << std::endl;
}

void Configuration::write(const std::string& filename) const
{
    std::ofstream config_file(filename);

    if (!config_file.is_open())
        return;

    config_file << *this;

    return;
}

const Configuration Configuration::default_configuration = Configuration();
