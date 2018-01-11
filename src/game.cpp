#include "game.hpp"
#include "menu/menus.hpp"
#include "ressources.hpp"


Game::Game() :
    move_time(1.f),
    map(nullptr),
    entities(nullptr),
    map_exploration(nullptr),
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
    window.setKeyRepeatEnabled(false);

    window.setVerticalSyncEnabled(config.vsync);
    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

    if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
        window.setFramerateLimit(config.maxfps);

    config.readGame(Configuration::data_path + "game.ini");
    RessourceManager::loadRessources(Configuration::data_path);
    StatManager::loadStats();

    move_time = 1.f / config.animation_speed;

    menu = std::make_shared<MainMenu>();
}

void Game::newGame(const std::string& save_path, Class hero_class)
{
    dungeon.clear();

    game_name = save_path;
    current_level = 0;
    entity_turn = EntityType::Hero;
    next_move = 0.f;

    // Seed the rng
    std::random_device r;
    Rand::seed(r());
    RandGen::seed(r());

    dungeon.push_back(Level());
    generators.push_back(new Generator(config.gen_options));
    exploration.emplace_back();

    map = &dungeon[0].map;
    entities = &dungeon[0].entities;
    generator = generators[0];
    map_exploration = &exploration[0];

    if (!map->hasChunk(0, 0))
    {
        map->setChunk(0, 0, generator->getChunkCells(0, 0));
        auto first_entities = generator->getChunkEntities(0, 0);
        entities->insert(end(dungeon[0].entities), begin(first_entities), end(first_entities));
    }

    sf::Vector2i start_pos;
    auto entry_stairs = std::find_if(entities->begin(), entities->end(),
    [](const std::shared_ptr<Entity> e) -> bool
    {
        return e->getType() == EntityType::Stairs && e->getInteraction() == Interaction::GoUp;
    });
    if (entry_stairs != entities->end())
        start_pos = (*entry_stairs)->getPosition();

    entities->push_back(std::make_shared<Character>(
        EntityType::Hero, Interaction::None, start_pos,
        Direction::Left, hero_class, 20, 1, Controller::Player1));
}

void Game::loadArround()
{
    // Find hero and center view
    auto hero = std::find_if(entities->begin(), entities->end(),
    [](const std::shared_ptr<Entity>& e)
    {
        return e->getType() == EntityType::Hero;
    });

    // Get adjacent chunks
    auto position = (*hero)->getPosition();
    auto chunk_position = Chunk::sector(position.x, position.y);
    sf::Vector2i chunk_id;

    // Preload further
    generator->generateRadius(chunk_position.first, chunk_position.second, DIST_CHUNK_LOAD);
    generator->preGenerateRadius(chunk_position.first, chunk_position.second, DIST_CHUNK_PRELOAD);

    for (chunk_id.x = chunk_position.first - DIST_CHUNK_LOAD ; chunk_id.x <= chunk_position.first + DIST_CHUNK_LOAD ; chunk_id.x++)
    {
        for (chunk_id.y = chunk_position.second - DIST_CHUNK_LOAD ; chunk_id.y <= chunk_position.second + DIST_CHUNK_LOAD ; chunk_id.y++)
        {
            if (!map->hasChunk(chunk_id.x, chunk_id.y))
            {
                int x = chunk_id.x;
                int y = chunk_id.y;

                map->setChunk(x, y, generator->getChunkCells(x, y));
                auto new_entities = generator->getChunkEntities(x, y);
                entities->insert(end(*entities), begin(new_entities), end(new_entities));
            }
        }
    }
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

            if (menu)
                menu->handleInput(event, config);
            else if (event.type == sf::Event::KeyPressed &&
                     event.key.code == config.menu_key)
                menu = std::make_shared<PauseMenu>();
        }

        float elapsed_time = timer.restart().asSeconds();

        // Update
        if (menu) {
            menu->update();
        }
        else {
            next_move -= elapsed_time;
            update(); // Update Game iff there is no Menu
            loadArround();
        }

        // Draw
        window.clear();
        if (!menu || menu->displayGame())
            render();

        if (menu)
        {
            renderer.setView(window);
            menu->render(window);
        }
        window.display();

        // Apply menu events
        MenuEvent menu_ev;
        if (menu)
            menu_ev = menu->menuEvent();

        switch (menu_ev.type)
        {
        case MenuEvent::Quit:
            window.close();
            break;

        case MenuEvent::Resume:
            menu = nullptr;
            break;

        case MenuEvent::NextMenu:
            menu = menu_ev.next_menu;
            break;

        case MenuEvent::NewGame:
            newGame(menu_ev.save_path, menu_ev.hero_class);
            menu = nullptr;
            break;

        case MenuEvent::LoadGame:
            if (loadGame(menu_ev.save_path))
                menu = nullptr;
            break;

        case MenuEvent::SaveGame:
            if (!config.gen_options.infinite) {
                // Load cached chunks
                for (size_t i_map = 0 ; i_map < dungeon.size() ; i_map++)
                {
                    for (const auto& id_chunk : generators[i_map]->getCachedChunks())
                    {
                        int x = id_chunk.first;
                        int y = id_chunk.second;

                        dungeon[i_map].map.setChunk(x, y, generators[i_map]->getChunkCells(x, y));

                        auto nentities = generators[i_map]->getChunkEntities(x, y);
                        dungeon[i_map].entities.insert(end(dungeon[i_map].entities), begin(nentities), end(nentities));
                    }
                }

                saveGame();
            }
            else
            {
                std::cout << "Not available yet for infinite maps" << std::endl;
            }
            break;

        default:
            break;
        }
    }
}

void Game::update()
{
    bool monster_acting = false;
    EntityType next_turn = EntityType::Hero;

    if (!config.monsters_no_delay && next_move > 0.f)
        return;
    else if (entity_turn == EntityType::Hero && next_move > 0.f)
        return;

    for (auto& entity : *entities)
    {
        if (config.monsters_no_delay)
            if (entity_turn == EntityType::Monster && entity->getType() == EntityType::Hero)
                continue;

        entity->setMoving(false);
        entity->setAttacking(false);
        entity->setAttacked(false);

        if (entity->getType() == EntityType::Hero)
        {
            auto hero = std::static_pointer_cast<Character>(entity);
            if (hero->getHp() <= 0)
                menu = std::make_shared<GameOverMenu>(hero->getLevel());
        }
    }

    for (auto& entity : *entities)
    {
        if (entity->getType() != entity_turn)
            continue;

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
                                    auto hero = std::find_if(dungeon[current_level].entities.begin(), dungeon[current_level].entities.end(),
                                        [](std::shared_ptr<Entity> e) -> bool {
                                            return e->getType() == EntityType::Hero;
                                        });

                                    if (current_level == dungeon.size()-1)
                                    {
                                        dungeon.push_back(Level());
                                        dungeon[current_level+1].entities.push_back(*hero);
                                        generators.push_back(new Generator(config.gen_options));
                                        exploration.emplace_back();

                                        dungeon[current_level+1].map.setChunk(0, 0, generators[current_level+1]->getChunkCells(0, 0));

                                        auto first_entities = generators[current_level+1]->getChunkEntities(0, 0);
                                        dungeon[current_level+1].entities.insert(end(dungeon[current_level+1].entities), begin(first_entities), end(first_entities));
                                    }

                                    current_level++;

                                    map = &dungeon[current_level].map;
                                    entities = &dungeon[current_level].entities;
                                    generator = generators[current_level];
                                    map_exploration = &exploration.back();

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
                                    map_exploration = &exploration[current_level];

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

            if (entity->getType() == EntityType::Hero && action.type != ActionType::Interact)
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
                return !e2->isAlive() && (e2->getType() != EntityType::Hero);
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

            if (map->hasCell(position.x, position.y) && map->cellAt(position) != CellType::Floor)
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

            auto targets = getEntitiesOnCell(position);
            for (auto& target : targets)
            {
                EntityType target_type = target->getType();
                if (target_type == EntityType::Hero || target_type == EntityType::Monster)
                {
                    target->setAttacked(true);

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

void Game::render()
{
    float frame_progress = 1.f - next_move / move_time;

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

    if (hero == entities->end())
        hero = entities->begin(); // Center on random (first) entity if hero not found

    renderer.drawGame(*map, *map_exploration, *entities, *hero, frame_progress, config);

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
