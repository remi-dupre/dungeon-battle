#include "render.hpp"

#include "lighting.hpp"
#include "ressources.hpp"

Renderer::Renderer() :
    seed(0)
{
    // Initialize the seed of the renderer
    seed = static_cast<unsigned int>
        (Rand::uniform_int(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));

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

void Renderer::drawMap(const Map& map)
{
    int map_w = map.getWidth();
    int map_h = map.getHeight();

    sf::Vector2i min_corner = static_cast<sf::Vector2i>
        (math::floor((view.getCenter() - 0.5f * view.getSize()) / tile_size));
    sf::Vector2i max_corner = static_cast<sf::Vector2i>
        (math::ceil((view.getCenter() + 0.5f * view.getSize()) / tile_size));
    sf::Vector2i hero_pos = (max_corner + min_corner) / 2;

    map_vertices.clear();
    map_vertices.reserve((max_corner.x - min_corner.x) * (max_corner.y - min_corner.y) * 4u);

    for (int x = std::max(min_corner.x, 0); x < std::min(max_corner.x, map_w); x++)
    {
        for (int y = std::max(min_corner.y, 0); y < std::min(max_corner.y, map_h); y++)
        {
            if (can_be_seen(hero_pos, {x, y}, map))
            {
                CellType cell = map.cellAt(x, y);
                drawCell({x, y}, cell, map);
            }
        }
    }
}

#include <iostream>

void Renderer::drawEntities(const std::vector<std::shared_ptr<Entity>>& entities, float frame_progress)
{
    entities_sprites.clear();
    entities_sprites.reserve(entities.size());

    for (const auto& entity : entities)
    {
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

    view.setSize({static_cast<float>(Configuration::default_configuration.width),
                static_cast<float>(Configuration::default_configuration.height)});
    view.setViewport({x_offset / 2.f, y_offset / 2.f, 1.f - x_offset, 1.f - y_offset});

    target.setView(view);

    sf::RenderStates map_rstates(&RessourceManager::getTexture(Textures::Tileset));
    target.draw(map_vertices.data(),
                map_vertices.size(),
                sf::PrimitiveType::Triangles,
                map_rstates);

    for (const auto& sprite : entities_sprites)
        target.draw(sprite);

    view.setCenter(static_cast<float>(Configuration::default_configuration.width) / 2.f,
                   static_cast<float>(Configuration::default_configuration.height) / 2.f);
    target.setView(view);
    target.draw(hero_life);
    target.draw(hero_xp);

    for (const auto& t : menu_texts)
        target.draw(t);
    menu_texts.clear();
}

void Renderer::drawCell(sf::Vector2i coords, CellType cell, const Map& map)
{
    if (cell == CellType::Empty)
        return;

    RandRender::seed(std::hash<sf::Vector2i>{}(coords));

    sf::Vector2f p = tile_size * static_cast<sf::Vector2f>(coords);

    sf::Vertex v1, v2, v3, v4;

    v1.position = p;
    v2.position = {p.x, p.y + tile_size};
    v3.position = {p.x + tile_size, p.y};
    v4.position = {p.x + tile_size, p.y + tile_size};

    // Texture rectangles for floor are hardcoded for now
    const sf::Vector2f tiles_coord[] = {
        {RandRender::uniform_int(0, 5) * 32.f, 128.f},
        {0.f, 96.f}, {32.f, 96.f}, {0.f, 64.f}, {64.f, 96.f},
        {32.f, 64.f}, {64.f, 64.f}, {0.f, 32.f}, {96.f, 96.f},
        {96.f, 64.f}, {128.f, 64.f}, {32.f, 32.f}, {160.f, 64.f},
        {64.f, 32.f}, {96.f, 32.f},
        {RandRender::uniform_int(0, 5) * 32.f, 0.f}
    };

    sf::Vector2i next_tiles[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    int tile_index = 0;

    for (int i = 0; i < 4; i++)
    {
        if (map.wallNext(coords + next_tiles[i]))
            tile_index |= 1 << i;
    }
    if (map.wallNext(coords))
        tile_index = 15;

    assert(0 <= tile_index && tile_index < 16);

    int tile_nb;

    sf::Vector2f t = tiles_coord[tile_index];

    switch (cell)
    {
        case CellType::Floor:
            v1.texCoords = t;
            v2.texCoords = {t.x, t.y + 31.f};
            v3.texCoords = {t.x + 31.f, t.y};
            v4.texCoords = {t.x + 31.f, t.y + 31.f};
            break;

        case CellType::Wall:
            tile_nb = RandRender::uniform_int(0, 3);
            v1.texCoords = {32.f * tile_nb, 160.f};
            v2.texCoords = {32.f * tile_nb, 191.f};
            v3.texCoords = {32.f * tile_nb + 31.f, 160.f};
            v4.texCoords = {32.f * tile_nb + 31.f, 191.f};
            break;

        case CellType::Empty:
            [[fallthrough]];
        default:
            return; // Don't draw empty/unkonwn cells
    }

    map_vertices.push_back(v1);
    map_vertices.push_back(v2);
    map_vertices.push_back(v4);

    map_vertices.push_back(v1);
    map_vertices.push_back(v3);
    map_vertices.push_back(v4);
}
