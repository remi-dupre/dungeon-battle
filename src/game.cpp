#include "game.hpp"


Game::Game() :
    map(nullptr),
    entities(nullptr),
    current_level(0),
    entity_turn(EntityType::Hero),
    next_move(0.f)
{}

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
    Rand::seed(r());

    current_level = 0;

    // Generate a map
    GenerationMode gen_options;
    gen_options.room_min_size = 50;
    gen_options.room_max_size = 300;
    gen_options.nb_rooms = 20;
    gen_options.room_margin = 2;
    gen_options.monster_load = 3.f;
    gen_options.maze_density = 0.1f;
    gen_options.type = LevelType::Cave;

    // Base stats of Heros and Monsters
    unsigned int baseHeroHp = 20;
    unsigned int baseMonsterHp = 5;
    unsigned int baseHeroForce = 1;
    unsigned int baseMonsterForce = 1;

    dungeon.push_back(generate(gen_options));

    map = &dungeon[0].map;
    entities = &dungeon[0].entities;

    auto entry_stairs = std::find_if(entities->begin(), entities->end(),
    [](const std::shared_ptr<Entity> e) -> bool
    {
        return e->getType() == EntityType::Stairs && e->getInteraction() == Interaction::GoUp;
    });

    sf::Vector2i start_pos;
    if (entry_stairs != entities->end())
        start_pos = (*entry_stairs)->getPosition();

    entities->push_back(std::make_shared<Character>(EntityType::Hero,
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

    for (auto& entity : *entities)
    {
        if (entity->getType() != entity_turn)
        {
            entity->setMoving(false);
            entity->setAttacking(false);
            continue;
        }

        Action action = control::get_input(*entity, *entities, *map, config);

        bool action_done = update_entity(entity, action);

        if (action_done)
        {
            switch (action.type)
            {
            case ActionType::Move:
                entity->setMoving(true);
                break;
            case ActionType::Attack:
                entity->setAttacking(true);
                break;
            case ActionType::Interact:
                {
                    for(auto& target : getEntitiesOnCell(entity->getPosition()))
                    {
                        if (target->getType() == EntityType::Stairs)
                        {
                            switch (target->getInteraction())
                            {
                                case Interaction::GoDown: {
                                    if (current_level == dungeon.size()-1)
                                    {
                                        // Generate a map
                                        GenerationMode gen_options;
                                        gen_options.room_min_size = 50;
                                        gen_options.room_max_size = 300;
                                        gen_options.nb_rooms = 20;
                                        gen_options.room_margin = 2;
                                        gen_options.monster_load = 3.f;
                                        gen_options.maze_density = 0.1f;
                                        gen_options.type = LevelType::Cave;
                                        dungeon.push_back(generate(gen_options));

                                    }

                                    auto hero = std::find_if(dungeon[current_level].entities.begin(),dungeon[current_level].entities.end(),
                                        [](std::shared_ptr<Entity> e) -> bool {
                                            return e->getType() == EntityType::Hero;
                                        });

                                    current_level++;

                                    dungeon[current_level].entities.push_back(*hero);
                                    map = &dungeon[current_level].map;
                                    entities = &dungeon[current_level].entities;

                                    auto stairs = std::find_if(dungeon[current_level].entities.begin(), dungeon[current_level].entities.end(),
                                        [](std::shared_ptr<Entity> e) -> bool {
                                            return e->getType() == EntityType::Stairs && e->getInteraction() == Interaction::GoUp;
                                        });
                                    
                                    (*hero)->setPosition((*stairs)->getPosition()); }

                                    break;

                                case Interaction::GoUp: {
                                    if (current_level == 0)
                                        break;

                                    current_level--;

                                    map = &dungeon[current_level].map;
                                    entities = &dungeon[current_level].entities;
                                
                                    auto hero = std::find_if(dungeon[current_level].entities.begin(),dungeon[current_level].entities.end(),
                                    [](std::shared_ptr<Entity> e) -> bool {
                                        return e->getType() == EntityType::Hero;
                                    });
                                
                                    auto stairs = std::find_if(dungeon[current_level].entities.begin(), dungeon[current_level].entities.end(),
                                    [](std::shared_ptr<Entity> e) -> bool {
                                        return e->getType() == EntityType::Stairs && e->getInteraction() == Interaction::GoDown;
                                    });
                                
                                    (*hero)->setPosition((*stairs)->getPosition()); }
                                    
                                    break;
                                
                                default:
                                    break;
                            }
                        }
                    }
                }
            default:
                break;
            }
        }

        if (action.type != ActionType::None)
        {
            monster_acting = true;

            if (entity->getType() == EntityType::Hero)
            {
                next_turn = EntityType::Monster;
            }
        }
    }

    entity_turn = next_turn;
    if (monster_acting) // No animation time for monsters if they do not move or attack
        next_move = 1.f / config.animation_speed;

    auto it = std::remove_if(entities->begin(), entities->end(),
        [](std::shared_ptr<Entity> e)
        {
            if (e->getType() == EntityType::Monster || e->getType() == EntityType::Hero)
            {
                auto e2 = std::static_pointer_cast<Character>(e);
                return !e2->isAlive();
            }
            return false;
        });

    entities->erase(it, entities->end());
}

bool Game::update_entity(std::shared_ptr<Entity> entity, Action action)
{
    sf::Vector2i position = entity->getPosition();

    entity->setOrientation(action.direction);

    switch (action.type)
    {
        case ActionType::Move:
        {
            position += to_vector2i(action.direction);

            if (map->cellAt(position) != CellType::Floor)
                return false; // Wall -> don't move

            auto entities_on_target = getEntitiesOnCell(position);
            auto entity_on_target = std::find_if(entities_on_target.begin(), entities_on_target.end(),
                [](const std::shared_ptr<Entity> e) -> bool
                {
                    EntityType t = e->getType();
                    return t == EntityType::Hero || t == EntityType::Monster;
                });

            if (entity_on_target != entities_on_target.end())
                return false; // Entity on target cell -> don't move

            entity->setPosition(position);

            return true;
        } break;

        case ActionType::Attack:
        {
            position += to_vector2i(action.direction);

            entity->setAttacking(true);
            entity->setOrientation(action.direction);

            for (auto& target : getEntitiesOnCell(position))
            {
                EntityType target_type = target->getType();

                if (target_type == EntityType::Hero || target_type == EntityType::Monster)
                {
                    auto s = std::static_pointer_cast<Character>(entity);
                    auto t = std::static_pointer_cast<Character>(target);

                    t->addHp(std::min(static_cast<int>(t->getDefense()) -
                                      static_cast<int>(s->getStrength()), -1));

                    return true;
                }
            }

            return true;
        } break;

        case ActionType::Interact:
            return true;
            break;

        default:
            break;
        }

    return false;
}

void Game::display()
{
    //Sort entities by zIndex and depth
    std::sort(entities->begin(), entities->end(),
        [](const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2)
        {
            return e1->zIndex() < e2->zIndex()
                || (e1->zIndex() == e2->zIndex() && e1->getPosition().y < e2->getPosition().y);
        }
    );

    auto hero = std::find_if(entities->begin(), entities->end(),
        [](const std::shared_ptr<Entity>& e)
        {
            return e->getType() == EntityType::Hero;
        });
    if (hero != entities->end())
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

    renderer.drawMap(*map);
    renderer.drawEntities(*entities, std::max(next_move, 0.f) * config.animation_speed);

    window.clear();
    renderer.display(window);
    window.display();
}

const std::vector<std::shared_ptr<Entity>>& Game::getEntities() const
{
    return *entities;
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
        std::begin(*entities), std::end(*entities),
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
        std::begin(*entities), std::end(*entities),
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
