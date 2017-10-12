#include <cmath>
#include <map>
#include <string>

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

    // Seed the rng
    std::random_device r;
    Random::seed(r());

    // Generate a map
    GenerationMode gen_options;
    gen_options.room_min_size = 50;
    gen_options.room_max_size = 300;
    gen_options.nb_rooms = 20;
    gen_options.room_margin = 2;
    gen_options.monster_load = 3.f;
    gen_options.type = LevelType::Cave;

    // Base stats of Heros and Monsters
    unsigned int baseHeroHp = 20;
    unsigned int baseMonsterHp = 5;
    unsigned int baseHeroForce = 1;
    unsigned int baseMonsterForce = 1;

    Level level = generate(gen_options);

    map = std::move(level.map);
    entities = std::move(level.entities);

    auto entry_stairs = std::find_if(entities.begin(), entities.end(),
    [](const std::shared_ptr<Entity> e) -> bool
    {
            return e->getType() == EntityType::Stairs;
    });

    sf::Vector2i start_pos;
    if (entry_stairs != entities.end())
        start_pos = (*entry_stairs)->getPosition();

    entities.push_back(std::make_shared<Character>(EntityType::Hero,
                                                   Interaction::None,
                                                   start_pos,
                                                   Direction::Left,
                                                   baseHeroHp,
                                                   baseHeroForce));

    entity_turn = EntityType::Hero;
    next_move = 0.f;
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

        next_move -= timer.restart().asSeconds();
        if (next_move <= 0.f)
        {
            update();
        }

        display();
    }
}

void Game::update()
{
    bool monster_acting = false;
    EntityType next_turn = EntityType::Hero;

    for (auto& entity : entities)
    {
        if (entity->getType() != entity_turn)
        {
            entity->setMoving(false);
            entity->setAttacking(false);
            continue;
        }

        Action action = control::get_input(*entity, entities, map, config);

        sf::Vector2i position = entity->getPosition();

        bool perform_action = false;

        switch (action.type)
        {
        case ActionType::Move:
            switch(action.direction)
            {
            case Direction::Left:
                if (position.x-- > 0)
                    perform_action = true;
                break;
            case Direction::Right:
                if (++position.x < map.getWidth())
                    perform_action = true;
                break;
            case Direction::Up:
                if (position.y-- > 0)
                    perform_action = true;
                break;
            case Direction::Down:
                if (++position.y < map.getHeight())
                    perform_action = true;
                break;
            default:
                break;
            }
            if (perform_action)
            {
                auto entities_on_target = getEntitiesOnCell(position);
                auto entity_on_target =
                    std::find_if(
                                 entities_on_target.begin(),
                                 entities_on_target.end(),
                                 [](const std::shared_ptr<Entity> e) -> bool
                                 {
                                     EntityType t = e->getType();
                                     return t == EntityType::Hero || t == EntityType::Monster;
                                 }
                                 );

                if (entity_on_target != entities_on_target.end())
                    perform_action = false;
                else if(map.cellAt(position.x, position.y) != CellType::Floor)
                    perform_action = false;
            }
            if (perform_action)
            {
                entity->setPosition(position);
            }
            entity->setOrientation(action.direction);
            break;
        case ActionType::Attack:
            perform_action = true;
            break;
        default:
            break;
        }

        if (perform_action)
        {
            switch (action.type)
            {
            case ActionType::Move:
                entity->setMoving(true);
                break;
            case ActionType::Attack:
                entity->setAttacking(true);
                break;
            default:
                break;
            }
        }

        if (entity->getType() == EntityType::Hero)
        {
            if (action.type != ActionType::None)
            {
                next_turn = EntityType::Monster;
                next_move = 1.f / config.animation_speed;
            }
        }
        else if (action.type != ActionType::None)
        {
            monster_acting = true;
        }

    }

    entity_turn = next_turn;
    if (monster_acting) // No animation time for monsters if they do not move or attack
        next_move = 1.f / config.animation_speed;
}

void Game::display()
{
    //Sort entities by zIndex and depth
    std::sort(entities.begin(), entities.end(),
              [](const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2){
                  return e1->zIndex() < e2->zIndex()
                      || (e1->zIndex() == e2->zIndex() && e1->getPosition().y < e2->getPosition().y);
    });

    auto hero = std::find_if(entities.begin(), entities.end(),
        [](const std::shared_ptr<Entity>& e)
        {
            return e->getType() == EntityType::Hero;
        });
    if (hero != entities.end())
    {
        sf::Vector2f view_center = static_cast<sf::Vector2f>((*hero)->getPosition());
        if ((*hero)->isMoving())
        {
            float frac = std::max(next_move, 0.f) * config.animation_speed;
            view_center += frac * (static_cast<sf::Vector2f>((*hero)->getOldPosition())
                                   - static_cast<sf::Vector2f>((*hero)->getPosition()));
        }
        renderer.setViewCenter(view_center);
    }

    renderer.drawMap(map);
    renderer.drawEntities(entities, std::max(next_move, 0.f) * config.animation_speed);

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
        [&position](const std::shared_ptr<Entity>& e) -> bool {
            return e->getPosition() == position;
        }
    );

    return entities_on_cell;
}

std::vector<std::shared_ptr<Entity>> Game::getEntitiesAroundCell(int x, int y, int d) const
{
    return getEntitiesAroundCell({x, y}, d);
}

std::vector<std::shared_ptr<Entity>> Game::getEntitiesAroundCell(sf::Vector2i position, int d) const
{
    std::vector<std::shared_ptr<Entity>> entities_on_cell;

    // Runs a copy on 'entities' outputed to 'entities_on_cell'
    std::copy_if(
        std::begin(entities), std::end(entities),
        std::back_inserter(entities_on_cell),
        [&position, d](const std::shared_ptr<Entity>& e) -> bool {
            sf::Vector2i p = e->getPosition();
            return (std::max(p.x, position.x) -
                    std::min(p.x, position.x) +
                    std::max(p.y, position.y) -
                    std::min(p.y, position.y)) <= d;
        }
    );

    return entities_on_cell;
}
