#include "render.hpp"

#include "lighting.hpp"
#include "ressources.hpp"

#pragma GCC diagnostic ignored "-Wdeprecated"


Renderer::Renderer() :
    seed(0)
{
    // Initialize the seed of the renderer
    seed = static_cast<unsigned int>
        (Rand::uniform_int(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));

    view.setSize({static_cast<float>(Configuration::default_configuration.width),
                static_cast<float>(Configuration::default_configuration.height)});


    // Max number of vertices at the same time
    unsigned int n_tiles = (Configuration::default_configuration.width + tile_size)
                         * (Configuration::default_configuration.height + tile_size)
                         / (tile_size * tile_size);

    map_vertices_bg.reserve(2 * 6 * n_tiles);
    map_vertices_fg.reserve(6 * n_tiles);
    entities_sprites.reserve(2 * n_tiles);

    hero_life.setFont(RessourceManager::getFont());
    hero_life.setCharacterSize(20.f);
    hero_life.setPosition(10.f, 10.f);
    hero_xp.setFont(RessourceManager::getFont());
    hero_xp.setCharacterSize(20.f);
    hero_xp.setPosition(10.f, 30.f);
}

void Renderer::setViewCenter(sf::Vector2f center)
{
    view.setCenter(tile_size * center);
}

void Renderer::drawMap(const Map& map, MapExploration& map_exploration)
{
    map_vertices_bg.clear();
    map_vertices_fg.clear();

    sf::Vector2i min_corner = sf::Vector2i(-1, -1) + static_cast<sf::Vector2i>
        (math::floor((view.getCenter() - 0.5f * view.getSize()) / tile_size));
    sf::Vector2i max_corner = sf::Vector2i(1, 1) + static_cast<sf::Vector2i>
        (math::ceil((view.getCenter() + 0.5f * view.getSize()) / tile_size));

    int map_w = map.getWidth();
    int map_h = map.getHeight();

    for (int x = std::max(min_corner.x, 0); x < std::min(max_corner.x, map_w); x++)
    {
        for (int y = std::max(min_corner.y, 0); y < std::min(max_corner.y, map_h); y++)
        {
            CellType cell = map.cellAt(x, y);
            drawCell({x, y}, cell, map, map_exploration);
        }
    }
}

void Renderer::drawEntities(const std::vector<std::shared_ptr<Entity>>& entities, float frame_progress)
{
    entities_sprites.clear();
    entities_sprites.reserve(entities.size());

    sf::FloatRect viewport = {math::floor((view.getCenter() - 0.5f * view.getSize()) / tile_size),
                              math::ceil((view.getCenter() + 0.5f * view.getSize()) / tile_size)};

    for (const auto& entity : entities)
    {

        if (!viewport.contains(static_cast<sf::Vector2f>(entity->getPosition())))
            continue;

        RandRender::seed(entity->getId() + seed);

        sf::Sprite entity_sprite;

        Textures texture_type = Textures::Tileset;
        EntitySprite entity_type = EntitySprite::None;
        sf::Color color = sf::Color::White;

        switch (entity->getType())
        {
        case EntityType::Stairs:
            texture_type = Textures::Scenery;
            if (entity->getInteraction() == Interaction::GoDown)
                entity_type = EntitySprite::StairsDown;
            if (entity->getInteraction() == Interaction::GoUp)
                entity_type = EntitySprite::StairsUp;
            break;

        case EntityType::Hero: {
            auto hero = std::static_pointer_cast<Character>(entity);
            hero_life.setString(std::to_string(hero->getHp()) + "/" +
                                std::to_string(hero->getHpMax()));
            hero_xp.setString("XP: " + std::to_string(hero->getExperience()) +
                              "\nLVL: " + std::to_string(hero->getLevel()));
        } [[fallthrough]];
        case EntityType::Monster: {
            auto character = std::static_pointer_cast<Character>(entity);
            switch (character->getClass())
            {
            case Class::Slime:
                texture_type = Textures::Slime;
                entity_type = EntitySprite::Slime;
                color.r = static_cast<sf::Uint8>(RandRender::uniform_int(0, 255));
                color.g = static_cast<sf::Uint8>(RandRender::uniform_int(0, 255));
                color.b = static_cast<sf::Uint8>(RandRender::uniform_int(0, 255));
                break;

            case Class::Knight:
                texture_type = Textures::Knight;
                entity_type = EntitySprite::Knight;
                break;

            case Class::Rogue:
                texture_type = Textures::Rogue;
                entity_type = EntitySprite::Rogue;
                break;

            case Class::Wizard:
                texture_type = Textures::Wizard;
                entity_type = EntitySprite::Wizard;
                break;
            default:
                break;
            }
            break;
        }
        case EntityType::None:
            [[fallthrough]];
        default:
            break;
        }

        entity_sprite.setTexture(RessourceManager::getTexture(texture_type)); // Texture

        // Animation
        float entity_frame_progress = 1.f;
        if (entity->isMoving() || entity->isAttacking())
            entity_frame_progress = frame_progress;

        EntityAnimationData animation = RessourceManager::getAnimation(entity_type);
        entity_sprite.setTextureRect(animation.getFrame(entity->getOrientation(), entity_frame_progress));

        // Origin
        entity_sprite.setOrigin(-static_cast<sf::Vector2f>(vec::position(animation.sprite_rect)));

        //Position
        sf::Vector2f pos = tile_size * static_cast<sf::Vector2f>(entity->getPosition());
        sf::Vector2f old_pos = tile_size * static_cast<sf::Vector2f>(entity->getOldPosition());

        if (entity->isMoving())
            pos = (pos - old_pos) * 0.5f * (1.f - std::cos(math::pi<float> * frame_progress)) + old_pos;
        entity_sprite.setPosition(pos);

        // Color
        entity_sprite.setColor(color);

        entities_sprites.push_back(entity_sprite);
    }
}

void Renderer::drawMenu(std::shared_ptr<const Menu> menu)
{
    menu_texts.clear();

    if (menu == nullptr)
        return;

    auto p = std::dynamic_pointer_cast<const PauseMenu>(menu);
    if (p != nullptr)
    {
        sf::Text resume;
        resume.setFont(RessourceManager::getFont());
        resume.setString("Resume");
        resume.setCharacterSize(20);
        resume.setColor(sf::Color::White);
        if (p->item == 0)
        {
            resume.setCharacterSize(25);
            resume.setColor(sf::Color::Yellow);
        }
        resume.setPosition({Configuration::default_configuration.width / 2.f
                    - resume.getLocalBounds().width / 2.f, 280.f});

        sf::Text quit;
        quit.setFont(RessourceManager::getFont());
        quit.setString("(Save) (TODO) & Quit");
        quit.setCharacterSize(20);
        quit.setColor(sf::Color::White);
        if (p->item == 1)
        {
            quit.setCharacterSize(25);
            quit.setColor(sf::Color::Yellow);
        }
        quit.setPosition({Configuration::default_configuration.width / 2.f
                    - quit.getLocalBounds().width / 2.f, 320.f});

        menu_texts.push_back(resume);
        menu_texts.push_back(quit);
    }
}

void Renderer::display(sf::RenderTarget& target)
{
    // Keep aspect ratio
    float screen_ratio = static_cast<float>(Configuration::default_configuration.height) /
        static_cast<float>(Configuration::default_configuration.width);

    float screen_x = static_cast<float>(target.getSize().x);
    float screen_y = static_cast<float>(target.getSize().y);

    float ratio = screen_y / screen_x;

    float x_offset = 0.f;
    float y_offset = 0.f;

    if (ratio > screen_ratio)
    {
        float y = screen_x * screen_ratio;
        y_offset = (screen_y - y) / screen_y;
    }
    else if (ratio < screen_ratio)
    {
        float x =  screen_y / screen_ratio;
        x_offset = (screen_x - x) / screen_x;
    }

    view.setViewport({x_offset / 2.f, y_offset / 2.f, 1.f - x_offset, 1.f - y_offset});

    target.setView(view);

    sf::RenderStates map_rstates(&RessourceManager::getTexture(Textures::Tileset));
    target.draw(map_vertices_bg.data(),
                map_vertices_bg.size(),
                sf::PrimitiveType::Triangles,
                map_rstates);

    for (const auto& sprite : entities_sprites)
        target.draw(sprite);

    target.draw(map_vertices_fg.data(),
                map_vertices_fg.size(),
                sf::PrimitiveType::Triangles,
                map_rstates);

    view.setCenter(static_cast<float>(Configuration::default_configuration.width) / 2.f,
                   static_cast<float>(Configuration::default_configuration.height) / 2.f);
    target.setView(view);
    target.draw(hero_life);
    target.draw(hero_xp);

    for (const auto& t : menu_texts)
        target.draw(t);
    menu_texts.clear();
}

void Renderer::drawCell(sf::Vector2i coords, CellType cell, const Map& map, MapExploration& map_exploration)
{
    if (cell == CellType::Empty)
        return;

    sf::Vector2i min_corner = static_cast<sf::Vector2i>
        (math::floor((view.getCenter() - 0.5f * view.getSize()) / tile_size));
    sf::Vector2i max_corner = static_cast<sf::Vector2i>
        (math::ceil((view.getCenter() + 0.5f * view.getSize()) / tile_size));
    sf::Vector2i hero_pos = (max_corner + min_corner) / 2;

    bool cell_visible = can_be_seen(hero_pos, coords, map);
    if (!cell_visible && !map_exploration.isExplored(coords))
        return;
    if (cell_visible)
        map_exploration.setExplored(coords);

    RandRender::seed(std::hash<sf::Vector2i>{}(coords));

    sf::Vector2f pos = tile_size * static_cast<sf::Vector2f>(coords);

    Direction floor_neighborhood = Direction::None;
    if (map.wallNext(coords))
    {
        floor_neighborhood = Direction::Down | Direction::Up | Direction::Left | Direction::Right;
    }
    else
    {
        for (Direction dir : directions)
        {
            if (map.wallNext(coords + to_vector2i(dir)))
                floor_neighborhood |= dir;
        }
    }

    sf::Vector2f tex_coords = RessourceManager::getTileTextureCoords(CellType::Floor, floor_neighborhood);

    sf::Color cell_color = sf::Color::White;
    if (!cell_visible && map_exploration.isExplored(coords))
        cell_color = {100, 100, 100};

    sf::Vertex v1, v2, v3, v4;
    v1 = v2 = v3 = v4 = {pos, cell_color, tex_coords};

    v2.position.y += tile_size;
    v3.position.x += tile_size;
    v4.position += {tile_size, tile_size};

    v2.texCoords.y += tile_size;
    v3.texCoords.x += tile_size;
    v4.texCoords += {tile_size, tile_size};

    map_vertices_bg.push_back(v1);
    map_vertices_bg.push_back(v2);
    map_vertices_bg.push_back(v4);

    map_vertices_bg.push_back(v1);
    map_vertices_bg.push_back(v3);
    map_vertices_bg.push_back(v4);


    if (cell != CellType::Wall)
        return;

    Direction neighborhood = Direction::None;
    for (Direction dir : directions)
    {
        if (map.cellAt(coords + to_vector2i(dir)) == CellType::Floor)
            neighborhood |= dir;
    }

    // Lower part of walls
    sf::Vector2f wall_tex_coords =
        RessourceManager::getTileTextureCoords(CellType::Empty, Direction::None);
    if (map.cellAt(coords + sf::Vector2i(0, 1)) == CellType::Empty)
        wall_tex_coords = RessourceManager::getTileTextureCoords(CellType::Wall, Direction::None)
            + sf::Vector2f(0.f, tile_size);
    if (has_direction(neighborhood, Direction::Down))
        wall_tex_coords = RessourceManager::getTileTextureCoords(CellType::Wall, neighborhood)
            + sf::Vector2f(0.f, tile_size);

    v1.texCoords = v2.texCoords = v3.texCoords = v4.texCoords = wall_tex_coords;
    v2.texCoords.y += tile_size;
    v3.texCoords.x += tile_size;
    v4.texCoords += {tile_size, tile_size};

    map_vertices_bg.push_back(v1);
    map_vertices_bg.push_back(v2);
    map_vertices_bg.push_back(v4);

    map_vertices_bg.push_back(v1);
    map_vertices_bg.push_back(v3);
    map_vertices_bg.push_back(v4);

    // Upper part of walls
    wall_tex_coords = RessourceManager::getTileTextureCoords(CellType::Wall, neighborhood);

    v1.position.y -= tile_size;
    v2.position.y -= tile_size;
    v3.position.y -= tile_size;
    v4.position.y -= tile_size;

    v1.texCoords = v2.texCoords = v3.texCoords = v4.texCoords = wall_tex_coords;
    v2.texCoords.y += tile_size;
    v3.texCoords.x += tile_size;
    v4.texCoords += {tile_size, tile_size};

    map_vertices_fg.push_back(v1);
    map_vertices_fg.push_back(v2);
    map_vertices_fg.push_back(v4);

    map_vertices_fg.push_back(v1);
    map_vertices_fg.push_back(v3);
    map_vertices_fg.push_back(v4);


    // Corners of walls
    for (int i = 0; i < 4; ++i)
    {
        Direction dir1 = directions[i], dir2 = directions[(i+1) & 3],
            dir_corner = dir1 | dir2;
        if (map.cellAt(coords + to_vector2i(dir1)) != CellType::Floor &&
            map.cellAt(coords + to_vector2i(dir2)) != CellType::Floor)
        {
            CellType corner_cell = map.cellAt(coords + to_vector2i(dir_corner));

            sf::Vector2f offset = {0.f, 0.f};
            float height = 0.f;

            if (has_direction(dir_corner, Direction::Right))
                offset.x += tile_size / 2.f;
            if (has_direction(dir_corner, Direction::Down))
            {
                offset.y += tile_size / 2.f;
                if (corner_cell == CellType::Floor)
                    height = tile_size;
            }

            v1.position = v2.position = v3.position = v4.position =
                pos + offset - sf::Vector2f(0.f, tile_size);
            v2.position.y += tile_size / 2.f + height;
            v3.position.x += tile_size / 2.f;
            v4.position += {tile_size / 2.f, tile_size / 2.f + height};

            sf::Vector2f corner_tex_coords = {256.f, 32.f};
            if (corner_cell != CellType::Floor)
                corner_tex_coords = {320.f, 128.f};

            v1.texCoords = v2.texCoords = v3.texCoords = v4.texCoords = corner_tex_coords + offset;
            v2.texCoords.y += tile_size / 2.f + height;
            v3.texCoords.x += tile_size / 2.f;
            v4.texCoords += {tile_size / 2.f, tile_size / 2.f + height};

            map_vertices_fg.push_back(v1);
            map_vertices_fg.push_back(v2);
            map_vertices_fg.push_back(v4);

            map_vertices_fg.push_back(v1);
            map_vertices_fg.push_back(v3);
            map_vertices_fg.push_back(v4);
        }
    }
}
