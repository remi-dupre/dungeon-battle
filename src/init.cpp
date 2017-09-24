#include <string>

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "init.hpp"


namespace init
{
    Configuration configuration(const std::string& filename)
    {
        Configuration config;

        config.read(filename);

        return config;
    }

    sf::RenderWindow* window(const Configuration& config)
    {
        sf::Uint32 style = sf::Style::Close;

        if (config.fullscreen)
            style |= sf::Style::Fullscreen;

        sf::RenderWindow* window = new sf::RenderWindow({config.width, config.height}, "Dungeon Battle", style);

        window->setVerticalSyncEnabled(config.vsync);
        window->setSize({config.scalefactor * config.width, config.scalefactor * config.height});

        if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
            window->setFramerateLimit(config.maxfps);

        return window;
    }
}