#include "render.hpp"

#include "lighting.hpp"
#include "ressources.hpp"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wattributes"


Renderer::Renderer() :
    seed(0),
    entity_center_view(nullptr)
{
    // Initialize the seed of the renderer
    seed = static_cast<unsigned int>
        (Rand::uniform_int(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));

    world_view_size = {
        static_cast<int>(std::ceil(Configuration::default_configuration.width / tile_size)) + 3,
        static_cast<int>(std::ceil(Configuration::default_configuration.height / tile_size)) + 3
    };

    view.setSize({static_cast<float>(Configuration::default_configuration.width),
                  static_cast<float>(Configuration::default_configuration.height)});

    // Max number of vertices at the same time
    unsigned int n_tiles = world_view_size.x * world_view_size.y;
    map_vertices_bg.reserve(2 * 6 * n_tiles);
    map_vertices_fg.reserve(4 * 6 * n_tiles);
    entities_sprites.reserve(2 * n_tiles);

    hero_life.setFont(RessourceManager::getFont());
    hero_life.setCharacterSize(20.f);
    hero_life.setPosition(10.f, 10.f);
    hero_xp.setFont(RessourceManager::getFont());
    hero_xp.setCharacterSize(20.f);
    hero_xp.setPosition(10.f, 30.f);
}

void Renderer::drawGame(const Map& map,
                        MapExploration& map_exploration,
                        const std::vector<std::shared_ptr<Entity>>& entities,
                        std::shared_ptr<Entity> center_entity,
                        float frame_progress,
                        const Configuration& config)
{
    map_vertices_bg.clear();
    map_vertices_fg.clear();
    entities_sprites.clear();

    entity_center_view = center_entity;

    // Draw the map
    sf::IntRect viewport = {entity_center_view->getPosition() - world_view_size / 2, world_view_size};

    for (int x = viewport.left; x < viewport.left + viewport.width; x++)
    {
        for (int y = viewport.top; y < viewport.top + viewport.height + 1; y++)
        {
            CellType cell = map.cellAt(x, y);
            drawCell({x, y}, cell, map, map_exploration, config);
        }
    }

    // Draw the entities
    for (const auto& entity : entities)
    {
        // Entity not on screen
        if (!viewport.contains(entity->getPosition()))
            continue;

        bool entity_visible =
            can_be_seen(entity->getPosition(), entity_center_view->getPosition(), map) ||
            (entity->isMoving() &&
                can_be_seen(entity->getOldPosition(), entity_center_view->getPosition(), map));
        bool entity_drawn = entity_visible ||
            (entity->getType() != EntityType::Monster &&
                map_exploration.isExplored(entity->getPosition()));

        if(!config.lighting)
            entity_visible = entity_drawn = true;

        if (!entity_drawn)
            continue;

        drawEntity(entity, entity_visible, frame_progress);
    }
}

void Renderer::setView(sf::RenderTarget& target)
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

    view.setCenter(static_cast<float>(Configuration::default_configuration.width) / 2.f,
                   static_cast<float>(Configuration::default_configuration.height) / 2.f);

    target.setView(view);
}

void Renderer::display(sf::RenderTarget& target, float frame_progress)
{
    setView(target);

    sf::Vector2f view_pos = tile_size * static_cast<sf::Vector2f>(entity_center_view->getPosition());
    if (entity_center_view->isMoving())
    {
        view_pos *= frame_progress;
        view_pos += tile_size *
            static_cast<sf::Vector2f>(entity_center_view->getOldPosition()) * (1.f - frame_progress);
    }

    view.setCenter(view_pos);
    target.setView(view);

    sf::RenderStates map_rstates(&RessourceManager::getTexture(Textures::Tileset));
    target.draw(map_vertices_bg.data(), map_vertices_bg.size(),
                sf::PrimitiveType::Triangles, map_rstates);

    for (const auto& sprite : entities_sprites)
        target.draw(sprite);

    target.draw(map_vertices_fg.data(), map_vertices_fg.size(),
                sf::PrimitiveType::Triangles, map_rstates);

    view.setCenter(static_cast<float>(Configuration::default_configuration.width) / 2.f,
                   static_cast<float>(Configuration::default_configuration.height) / 2.f);
    target.setView(view);
    target.draw(hero_life);
    target.draw(hero_xp);
}

void Renderer::drawEntity(std::shared_ptr<Entity> entity,
                          bool cell_visible, float frame_progress)
{
    RandRender::seed(entity->getId() + seed);

    entities_sprites.emplace_back();
    sf::Sprite& entity_sprite = entities_sprites.back();

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

        case Class::Warrior:
            texture_type = Textures::Warrior;
            entity_type = EntitySprite::Warrior;
            break;

        case Class::Bat:
            texture_type = Textures::Bat;
            entity_type = EntitySprite::Bat;
            break;

        case Class::Goat:
            texture_type = Textures::Goat;
            entity_type = EntitySprite::Goat;
            break;

        case Class::Rabbit:
            texture_type = Textures::Rabbit;
            entity_type = EntitySprite::Rabbit;
            break;

        case Class::Rogue:
            texture_type = Textures::Rogue;
            entity_type = EntitySprite::Rogue;
            break;

        case Class::Wizard:
            texture_type = Textures::Wizard;
            entity_type = EntitySprite::Wizard;
            break;

        case Class::Angel:
            texture_type = Textures::Angel;
            entity_type = EntitySprite::Angel;
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
        entity_frame_progress = std::max(1.f - frame_progress, 0.f);
    if (entity->getType() == EntityType::Monster || entity->getType() == EntityType::Hero)
    {
        auto character = std::static_pointer_cast<Character>(entity);
        if (character->getClass() == Class::Bat) // Animate Bats when idle
            entity_frame_progress = frame_progress / 2.f;
    }

    EntityAnimationData& animation = RessourceManager::getAnimation(entity_type);
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
    float attack_ratio = 0.5f - 0.5f * frame_progress;
    if (entity->isAttacked())
    {
        color.g *= attack_ratio;
        color.b *= attack_ratio;
    }

    color *= {cell_shade[entity->getPosition()], cell_shade[entity->getPosition()], cell_shade[entity->getPosition()]};
    entity_sprite.setColor(color);
}

void Renderer::drawCell(sf::Vector2i coords, CellType cell, const Map& map, MapExploration& map_exploration, const Configuration& config)
{
    if (cell == CellType::Empty)
        return;

    sf::Vector2i hero_pos = entity_center_view->getPosition();
    bool cell_visible = can_be_seen(hero_pos, coords, map);
    bool next_visible = can_be_seen(hero_pos, coords + to_vector2i(Direction::Up), map) ||
                        can_be_seen(hero_pos, coords + to_vector2i(Direction::Down), map) ||
                        can_be_seen(hero_pos, coords + to_vector2i(Direction::Left), map) ||
                        can_be_seen(hero_pos, coords + to_vector2i(Direction::Right), map);
    bool wall_visible = cell_visible || (next_visible && cell == CellType::Wall);
    bool cell_explored = map_exploration.isExplored(coords);

    if(!config.lighting){
        cell_visible = next_visible = wall_visible = cell_explored = true;
    }

    if (wall_visible)
        map_exploration.setExplored(coords);
    if (!wall_visible && !cell_explored)
        return;

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

    if (cell_visible || (cell == CellType::Wall && next_visible))
        cell_shade[coords] = std::min(255, cell_shade[coords] + 5);
    else if (cell_explored && cell_shade[coords] < 100)
        cell_shade[coords] = std::min(100, cell_shade[coords] + 2);
    else if (cell_explored && cell_shade[coords] > 100)
        cell_shade[coords] = std::max(100, cell_shade[coords] - 2);


    sf::Vertex v1, v2, v3, v4;
    sf::Color cell_color = {cell_shade[coords], cell_shade[coords], cell_shade[coords]};
    v1 = v2 = v3 = v4 = {pos, cell_color, tex_coords};

    v2.position.y += tile_size;
    v3.position.x += tile_size;
    v4.position += {tile_size, tile_size};

    v2.texCoords.y += tile_size;
    v3.texCoords.x += tile_size;
    v4.texCoords += {tile_size, tile_size};

    if (cell_visible || cell_explored)
    {
        map_vertices_bg.push_back(v1);
        map_vertices_bg.push_back(v2);
        map_vertices_bg.push_back(v4);

        map_vertices_bg.push_back(v1);
        map_vertices_bg.push_back(v3);
        map_vertices_bg.push_back(v4);
    }

    if (cell != CellType::Wall)
        return;

    Direction wall_neighborhood = Direction::None;
    for (Direction dir : directions)
    {
        if (map.cellAt(coords + to_vector2i(dir)) == CellType::Floor)
            wall_neighborhood |= dir;
    }

    // Lower part of walls
    sf::Vector2f wall_tex_coords =
        RessourceManager::getTileTextureCoords(CellType::Empty, Direction::None);
    if (map.cellAt(coords + sf::Vector2i(0, 1)) == CellType::Empty ||
        !map_exploration.isExplored(coords + sf::Vector2i(0, 1)))
        wall_tex_coords = RessourceManager::getTileTextureCoords(CellType::Wall, Direction::None)
            + sf::Vector2f(0.f, tile_size);
    if (has_direction(wall_neighborhood, Direction::Down))
        wall_tex_coords = RessourceManager::getTileTextureCoords(CellType::Wall, wall_neighborhood)
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
    wall_tex_coords = RessourceManager::getTileTextureCoords(CellType::Wall, wall_neighborhood);

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
