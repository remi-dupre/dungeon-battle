#include <cmath>
#include <map>
#include <string>

#include "args.hpp"
#include "control.hpp"
#include "game.hpp"
#include "map.hpp"
#include "utility.hpp"


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

    // Seed the rng
    std::random_device r;
    Random::seed(r());

    // Generate a map
    GenerationMode gen_options;
    gen_options.room_min_size = 100;
    gen_options.room_max_size = 2000;
    gen_options.nb_rooms = 10;

    auto level = generate(gen_options);
    map = std::get<Map>(level);
    entities  = std::get<std::vector<std::shared_ptr<Entity>>>(level);

    entities.push_back(std::make_shared<Entity>(EntityType::Hero, sf::Vector2u(0, 0), Direction::Left));

    map.saveToFile("map.map");
}

void Game::run()
{
    sf::Clock timer;

    float time_since_last_update;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        time_since_last_update += timer.restart().asSeconds();

        if (time_since_last_update > 1.f / 10.f)
        {
            update();
            time_since_last_update -= 1.f / 10.f;
        }

        display();
    }
}

void Game::update()
{
    for (auto& entity : entities)
    {
        Action action = control::get_input(
            *entity,
            entities,
            [this](unsigned int x, unsigned int y) -> CellType {
                if (x < this->map.getWidth() && y < this->map.getHeight())
                    return this->map.cellAt(x, y);
                return CellType::Empty;
            },
            config
        );

        sf::Vector2u position = entity->getPosition();

        switch (action.type)
        {
        case ActionType::Move:
            switch(action.direction)
            {
            case Direction::Left:
                if (position.x-- > 0)
                    if(map.cellAt(position.x, position.y) != CellType::Wall)
                        entity->setPosition(position);
                break;
            case Direction::Right:
                if (++position.x < map.getWidth())
                    if (map.cellAt(position.x, position.y) != CellType::Wall)
                        entity->setPosition(position);
                break;
            case Direction::Up:
                if (position.y-- > 0)
                    if (map.cellAt(position.x, position.y) != CellType::Wall)
                        entity->setPosition(position);
                break;
            case Direction::Down:
                if (++position.y < map.getHeight())
                    if (map.cellAt(position.x, position.y) != CellType::Wall)
                        entity->setPosition(position);
                break;
            default:
                break;
            }
            entity->setOrientation(action.direction);
            break;
        default:
            break;
        }

        if (entity->getType() == EntityType::Hero)
        {
            renderer.setViewCenter(entity->getPosition());
        }
    }
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

std::vector<std::shared_ptr<Entity>> Game::getEntitiesOnCell(unsigned int x, unsigned int y) const
{
    return getEntitiesOnCell({x, y});
}

std::vector<std::shared_ptr<Entity>> Game::getEntitiesOnCell(sf::Vector2u position) const
{
    std::vector<std::shared_ptr<Entity>> entities_on_cell;

    // Runs a copy on 'entities' outputed to 'entities_on_cell'
    std::copy_if(
        std::begin(entities), std::end(entities),
        std::back_inserter(entities_on_cell),
        [&position](const std::shared_ptr<Entity>& e) -> bool {
            return e->getPosition() == position;
        }
    );

    return entities_on_cell;
}

std::vector<std::shared_ptr<Entity>> Game::getEntitiesAroundCell(unsigned int x, unsigned int y, unsigned int d) const
{
    return getEntitiesAroundCell({x, y}, d);
}

std::vector<std::shared_ptr<Entity>> Game::getEntitiesAroundCell(sf::Vector2u position, unsigned int d) const
{
    std::vector<std::shared_ptr<Entity>> entities_on_cell;

    // Runs a copy on 'entities' outputed to 'entities_on_cell'
    std::copy_if(
        std::begin(entities), std::end(entities),
        std::back_inserter(entities_on_cell),
        [&position, d](const std::shared_ptr<Entity>& e) -> bool {
            sf::Vector2u p = e->getPosition();
            return std::max(p.x, position.x) - std::min(p.x, position.x) + std::max(p.y, position.y) - std::min(p.y, position.y) <= d;
        }
    );

    return entities_on_cell;
}
