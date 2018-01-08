#include "game.hpp"
#include "menu/menus.hpp"
#include "ressources.hpp"


Game::Game() :
    move_time(1.f),
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
    window.setKeyRepeatEnabled(false);

    window.setVerticalSyncEnabled(config.vsync);
    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

    if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
        window.setFramerateLimit(config.maxfps);

    config.readGame(Configuration::data_path + "game.ini");
    RessourceManager::loadRessources(Configuration::data_path);

    move_time = 1.f / config.animation_speed;

    // Seed the rng
    std::random_device r;
    Rand::seed(r());

    menu = std::make_shared<MainMenu>();
}

void Game::newGame(const std::string& save_path, Class hero_class)
{
    dungeon.clear();

    game_name = save_path;
    current_level = 0;
    entity_turn = EntityType::Hero;
    next_move = 0.f;

    dungeon.push_back(generate(config.gen_options));
    exploration.emplace_back();

    map = &dungeon[0].map;
    entities = &dungeon[0].entities;
    map_exploration = &exploration[0];

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
            if (next_move <= 0.f)
                update(); // Update Game iff there is no Menu
        }

        // Draw
        window.clear();
        if (!menu || menu->displayGame())
            render();
        if (menu)
            menu->render(window);
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
            saveGame();
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

    for (auto& entity : *entities)
    {
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
                                        dungeon.push_back(generate(config.gen_options));
                                        dungeon[current_level+1].entities.push_back(*hero);
                                        exploration.emplace_back();
                                    }

                                    current_level++;

                                    map = &dungeon[current_level].map;
                                    entities = &dungeon[current_level].entities;
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

    renderer.drawGame(*map, *map_exploration, *entities, *hero, frame_progress);

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
