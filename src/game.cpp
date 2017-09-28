#include <map>
#include <string>

#include "args.hpp"
#include "game.hpp"


Game::Game() {}

void Game::init(const std::map<Option, std::string>& options)
{
    Configuration config;
    config.read(options.at(Option::Config));

    sf::Uint32 style = sf::Style::Close | sf::Style::Resize;
    if (config.fullscreen)
        style |= sf::Style::Fullscreen;

    window.create({config.width, config.height}, "Dungeon Battle", style);

    window.setVerticalSyncEnabled(config.vsync);
    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

    if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
        window.setFramerateLimit(config.maxfps);

    map = std::make_unique<Map>(20, 20);

    map->loadFromFile("map1.map");
}

void Game::run()
{
    sf::Clock timer;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float time_since_last_frame = timer.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            renderer.view.move(0.0f, -0.1f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            renderer.view.move(0.0f, 0.1f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            renderer.view.move(-0.1f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            renderer.view.move(0.1f, 0.0f);
        }

        display();
    }
}

void Game::update()
{
}

void Game::display()
{
    if (map)
        renderer.drawMap(*map);

    window.clear();
    renderer.display(window);
    window.display();
}
