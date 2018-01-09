#include <fstream>
#include <stdexcept>

#include "config.hpp"


sf::Keyboard::Key string_to_key(const std::string& name);


const Configuration Configuration::default_configuration = Configuration();

#ifdef PACKAGE
    const std::string Configuration::data_path = "/usr/share/dungeon-battle/";
    const std::string Configuration::user_path = "~/.dungeon-battle/";
#else
    const std::string Configuration::data_path = "data/";
    const std::string Configuration::user_path = "./";
#endif

// Default configuration
Configuration::Configuration() :
    gen_options()
{}

void Configuration::read(const std::string& filename)
{
    std::ifstream config_file(filename);
    std::string line;

    #ifdef PACKAGE
        system(("mkdir -p " + Configuration::user_path).c_str());
    #endif

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
                height = static_cast<unsigned int>(std::stoi(value));
            else if (option_name == "width")
                width = static_cast<unsigned int>(std::stoi(value));
            else if (option_name == "scalefactor")
                scalefactor = static_cast<unsigned int>(std::stoi(value));
            else if (option_name ==  "fullscreen")
                fullscreen = std::stoi(value);
            else if (option_name ==  "vsync")
                vsync = std::stoi(value);
            else if (option_name ==  "maxfps")
                maxfps = static_cast<unsigned int>(std::stoi(value));
            else if (option_name ==  "animation_speed")
                animation_speed = std::stof(value);
            else if (option_name == "lighting")
                lighting = std::stoi(value);
            else if (option_name == "monsters_no_delay")
                monsters_no_delay = std::stoi(value);
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
            else if (option_name == "infinite")
                gen_options.infinite = static_cast<bool>(std::stoi(value));
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
        "[Video]"                                    << "\n" <<
        "width="           << config.width           << "\n" <<
        "height="          << config.height          << "\n" <<
        "scalefactor="     << config.scalefactor     << "\n" <<
        "fullscreen="      << config.fullscreen      << "\n" <<
        "vsync="           << config.vsync           << "\n" <<
        "maxfps="          << config.maxfps          << "\n" <<
        "animation_speed=" << config.animation_speed << "\n" <<
        "lighting="        << config.lighting        << "\n" ;
}

void Configuration::write(const std::string& filename) const
{
    std::ofstream config_file(filename);

    if (!config_file.is_open())
        return;

    config_file << *this;

    return;
}

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
    else if (name == "Return" || name == "return" || name == "Enter" || name == "enter")
    {
        return sf::Keyboard::Key::Return;
    }

    return sf::Keyboard::Key::Unknown;
}
