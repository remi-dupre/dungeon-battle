#include "game.hpp"

#include "ressources.hpp"


Game::Game() :
    map(nullptr),
    entities(nullptr),
    current_level(0),
    entity_turn(EntityType::Hero),
    next_move(0.f),
    move_time(1.f)
{}

void Game::init(const std::map<Option, std::string>& options)
{
    config.read(options.at(Option::Config));

    sf::Uint32 style = sf::Style::Close | sf::Style::Resize;
    if (config.fullscreen)
        style |= sf::Style::Fullscreen;

    window.create({config.width, config.height}, "Dungeon Battle", style);
    window.setKeyRepeatEnabled(false);

    window.setVerticalSyncEnabled(config.vsync);
    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

    if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
        window.setFramerateLimit(config.maxfps);

#ifdef PACKAGE
    RessourceManager::loadRessources("/usr/share/dungeon-battle/data/");
#else
    RessourceManager::loadRessources("data/");
#endif

    menu = nullptr;

#ifdef PACKAGE
    config.readGame("/usr/share/dungeon-battle/game.ini");
#else
    config.readGame("data/game.ini");
#endif

    move_time = 1.f / config.animation_speed;

    // Seed the rng
    std::random_device r;
    Rand::seed(r());

    current_level = 0;

    // Base stats of Heros and Monsters
    unsigned int baseHeroHp = 20;
    unsigned int baseHeroForce = 1;

    dungeon.push_back(generate(config.gen_options));

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
                                                    baseHeroForce,
                                                    1));

    entity_turn = EntityType::Hero;
    next_move = 0.f;
}

void Game::run()
{
    sf::Clock timer;

#ifndef NDEBUG
    float time = 0.f;
    int frames = 0;
#endif

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (menu)
                    if (menu->handle_key(event.key.code, config))
                        window.close();
            }
        }

        float elapsed_time = timer.restart().asSeconds();
        next_move -= elapsed_time;

        if (menu)
        {
            if (menu->update())
                menu = menu->next_menu();
        }
        else if (next_move <= 0.f)
        {
            update(); // Update Game iff there is no Menu
        }

        window.clear();

        if (!menu)
        {
            display();
        }
        else
        {
            if (menu->display_game())
                display();
            renderer.drawMenu(std::const_pointer_cast<const Menu>(menu));
        }

        window.display();

#ifndef NDEBUG
        frames++;
        time += elapsed_time;
        if (time >= 1.f)
        {
            std::cout << frames << "fps" << std::endl;
            frames = 0;
            time = 0.f;
        }
#endif

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

        if (action.type == ActionType::Pause)
        {
            menu = std::make_shared<PauseMenu>();
        }

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
                                    auto hero = std::find_if(dungeon[current_level].entities.begin(), dungeon[current_level].entities.end(),
                                        [](std::shared_ptr<Entity> e) -> bool {
                                            return e->getType() == EntityType::Hero;
                                        });

                                    if (current_level == dungeon.size()-1)
                                    {
                                        dungeon.push_back(generate(config.gen_options));
                                        dungeon[current_level+1].entities.push_back(*hero);
                                    }

                                    current_level++;

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

                                    auto hero = std::find_if(dungeon[current_level].entities.begin(),
                                                             dungeon[current_level].entities.end(),
                                        [](std::shared_ptr<Entity> e) -> bool {
                                            return e->getType() == EntityType::Hero;
                                        });

                                    auto stairs = std::find_if(dungeon[current_level].entities.begin(),
                                                               dungeon[current_level].entities.end(),
                                        [](std::shared_ptr<Entity> e) -> bool {
                                            return e->getType() == EntityType::Stairs &&
                                                e->getInteraction() == Interaction::GoDown;
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
        next_move = move_time;

    // Remove dead entities
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
                    s->awardExperience(*t);

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
    float frame_progress = 1.f - std::max(next_move, 0.f) / move_time;

    //Sort entities by zIndex and depth
    std::sort(entities->begin(), entities->end(),
        [](const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2)
        {
            return e1->zIndex() < e2->zIndex()
                || (e1->zIndex() == e2->zIndex() && e1->getPosition().y < e2->getPosition().y);
        }
    );

    // Find hero and center view
    auto hero = std::find_if(entities->begin(), entities->end(),
        [](const std::shared_ptr<Entity>& e)
        {
            return e->getType() == EntityType::Hero;
        });

    if (hero != entities->end())
        renderer.setViewPos(*hero);

    // Draw map & entities
    renderer.drawMap(*map, map_exploration, frame_progress);
    renderer.drawEntities(*entities, frame_progress);

    renderer.display(window, frame_progress);
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
