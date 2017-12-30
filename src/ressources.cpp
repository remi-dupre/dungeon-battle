#include "ressources.hpp"

#include <cmath>
#include <fstream>
#include <string>


std::string RessourceManager::ressources_path = "data/"s;

std::map<Textures, sf::Texture> RessourceManager::textures;
std::map<EntitySprite, EntityAnimationData> RessourceManager::animations;
sf::Font RessourceManager::font;


sf::IntRect EntityAnimationData::getFrame(Direction dir, float frame_ratio)
{
    const auto& frames = animation[dir];
    return frames[std::fmod(std::floor(frames.size() * frame_ratio), frames.size())];
}

bool RessourceManager::loadRessources(const std::string& ressources_path_)
{
    ressources_path = ressources_path_;

    bool ok = true;

    ok &= loadTextures();
    ok &= loadFont();
    ok &= loadAnimations();

    return ok;
}

bool RessourceManager::loadTextures()
{
    sf::Texture& tileset = textures[Textures::Tileset];
    if (!tileset.loadFromFile(ressources_path + "tileset.png"))
        return false;

    // sf::Texture& scenery = textures[Textures::Scenery];
    // if (!scenery.loadFromFile(ressources_path + "entities.png"))
    //     return false;

    sf::Texture& character01 = textures[Textures::Knight];
    if (!character01.loadFromFile(ressources_path + "character01.png"))
        return false;

    sf::Texture& character02 = textures[Textures::Rogue];
    if (!character02.loadFromFile(ressources_path + "character01.png"))
        return false;

    sf::Texture& character03 = textures[Textures::Wizard];
    if (!character03.loadFromFile(ressources_path + "character01.png"))
        return false;

    // sf::Texture& character04 = textures[Textures::Character04];
    // if (!character04.loadFromFile(ressources_path + "character01.png"))
    //     return false;

    // sf::Texture& character05 = textures[Textures::Character05];
    // if (!character05.loadFromFile(ressources_path + "character01.png"))
    //     return false;

    sf::Texture& slime = textures[Textures::Slime];
    if (!slime.loadFromFile(ressources_path + "entities.png"))
        return false;

    return true;
}

bool RessourceManager::loadFont()
{
    if (!font.loadFromFile(ressources_path + "FSEX300.ttf"))
        return false;

    return true;
}

void compute_animation(sf::Vector2i sprite_size, sf::Vector2i frame_start, int nb_frames,
                       std::vector<sf::IntRect>& frames)
{
    for (int i = 0; i < nb_frames; ++i)
    {
        sf::IntRect frame;
        frame.left = frame_start.x;
        frame.top = frame_start.y;
        frame.width = sprite_size.x;
        frame.height = sprite_size.y;

        frame.left += sprite_size.x * i;
        frames.push_back(frame);
    }
}

bool RessourceManager::loadAnimations()
{
    std::ifstream animations_file(ressources_path + "animations");
    std::string entity_type_str;

    if (!animations_file.is_open())
        return false;

    animations.clear();

    while (!animations_file.eof())
    {
        animations_file >> entity_type_str;
        EntitySprite entity_type = EntitySprite::None;
        if (entity_type_str == "Slime")
            entity_type = EntitySprite::Slime;
        else if (entity_type_str == "Knight")
            entity_type = EntitySprite::Knight;
        else if (entity_type_str == "Rogue")
            entity_type = EntitySprite::Rogue;
        else if (entity_type_str == "Wizard")
            entity_type = EntitySprite::Wizard;
        else if (entity_type_str == "StairsUp")
            entity_type = EntitySprite::StairsUp;
        else if (entity_type_str == "StairsDown")
            entity_type = EntitySprite::StairsDown;

        EntityAnimationData animation_data;

        animations_file >> animation_data.sprite_rect.left;
        animations_file >> animation_data.sprite_rect.top;
        animations_file >> animation_data.sprite_rect.width;
        animations_file >> animation_data.sprite_rect.height;

        sf::Vector2i start;
        int nb_frames;


        animations_file >> start.x >> start.y >> nb_frames;
        compute_animation(vec::size(animation_data.sprite_rect), start, nb_frames,
                          animation_data.animation[Direction::Down]);

        animations_file >> start.x >> start.y >> nb_frames;
        compute_animation(vec::size(animation_data.sprite_rect), start, nb_frames,
                          animation_data.animation[Direction::Left]);

        animations_file >> start.x >> start.y >> nb_frames;
        compute_animation(vec::size(animation_data.sprite_rect), start, nb_frames,
                          animation_data.animation[Direction::Right]);

        animations_file >> start.x >> start.y >> nb_frames;
        compute_animation(vec::size(animation_data.sprite_rect), start, nb_frames,
                          animation_data.animation[Direction::Up]);

        animations[entity_type] = animation_data;

        if (!animations_file)
            return false;

        animations_file >> std::ws;
    }

    return true;
}


sf::Texture& RessourceManager::getTexture(Textures texture_type)
{
    return textures[texture_type];
}

sf::Font& RessourceManager::getFont()
{
    return font;
}

sf::IntRect RessourceManager::getSpriteRect(EntitySprite entity_type)
{
    return animations[entity_type].sprite_rect;
}

EntityAnimationData& RessourceManager::getAnimation(EntitySprite entity_type)
{
    return animations[entity_type];
}
