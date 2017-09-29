#include <map>
#include <string>

#include "args.hpp"
#include "control.hpp"
#include "game.hpp"


Game::Game() {}

void Game::init(const std::map<Option, std::string>& options)
{
    config.read(options.at(Option::Config));

    sf::Uint32 style = sf::Style::Close | sf::Style::Resize;
    if (config.fullscreen)
        style |= sf::Style::Fullscreen;

    window.create({config.width, config.height}, "Dungeon Battle", style);

    window.setVerticalSyncEnabled(config.vsync);
    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

    if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
        window.setFramerateLimit(config.maxfps);

    map.loadFromFile("map1.map");
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

        for (auto& entity : entities)
        {
            Action action = control::get_input(*entity, entities,
                                               [this](unsigned int x, unsigned int y) -> const CellType& {
                                                   return this->map.cellAt(x, y);
                                               },
                                               config);
        }

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
    renderer.drawMap(map);
    renderer.drawEntities(entities);

    window.clear();
    renderer.display(window);
    window.display();
}

const std::vector<std::shared_ptr<Entity>>& Game::getEntities() const
{
    return entities;
}

std::vector<std::shared_ptr<Entity>> Game::getEntitiesOnCell(int x, int y) const
{
    return getEntitiesOnCell({x, y});
}

std::vector<std::shared_ptr<Entity>> Game::getEntitiesOnCell(sf::Vector2i position) const
{
    std::vector<std::shared_ptr<Entity>> entities_on_cell;
    // Runs a copy on 'entities' outputed to 'entities_on_cell'
    std::copy_if(
        std::begin(entities), std::end(entities),
        std::back_inserter(entities_on_cell),
        [position](const std::shared_ptr<Entity>& e) -> bool {
            return e->getPosition() == position;
        }
    );
    return entities_on_cell;
}
