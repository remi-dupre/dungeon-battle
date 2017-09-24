#include "game.hpp"


Game::Game()
{
}

void Game::init()
{
    std::string config_filename = "config.ini";
    config.read(config_filename);

    sf::Uint32 style = sf::Style::Close;
    if (config.fullscreen)
        style |= sf::Style::Fullscreen;

    window.create({config.width, config.height}, "Dungeon Battle", style);

    window.setVerticalSyncEnabled(config.vsync);
    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

    if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
        window.setFramerateLimit(config.maxfps);

    map = std::make_unique<Map>(20, 20);

    map->cellAt(2, 3) = CellType::Wall;
    map->cellAt(3, 3) = CellType::Wall;
    map->cellAt(4, 3) = CellType::Wall;
    map->cellAt(7, 8) = CellType::Wall;
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
