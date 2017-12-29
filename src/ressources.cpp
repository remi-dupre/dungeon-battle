#include "ressources.hpp"

#include <cmath>
#include <fstream>
#include <string>


std::map<Textures, sf::Texture> RessourceManager::textures;
sf::Font RessourceManager::font;
std::map<Class, EntityAnimationData> RessourceManager::animations;

#ifdef PACKAGE
const std::string RessourceManager::ressource_path_prefix = "/usr/share/dungeon-battle/"s;
#else
const std::string RessourceManager::ressource_path_prefix = ""s;
#endif


TextureRect EntityAnimationData::getFrame(Direction dir, float frame_ratio)
{
    const auto& frames = animation[dir];
    return frames[std::fmod(std::floor(frames.size() * frame_ratio), frames.size())];
}

bool RessourceManager::loadRessources()
{
    if (!loadTextures())
        return false;
    if (!loadFont())
        return false;
    if (!loadAnimations())
        return false;

    return true;
}

bool RessourceManager::loadTextures()
{
    sf::Texture& tileset = textures[Textures::Tileset];
    if (!tileset.loadFromFile(ressource_path_prefix + "data/tileset.png"))
        return false;

    sf::Texture& scenery = textures[Textures::Scenery];
    if (!scenery.loadFromFile(ressource_path_prefix + "data/entities.png"))
        return false;

    sf::Texture& character01 = textures[Textures::Character01];
    if (!character01.loadFromFile(ressource_path_prefix + "data/character01.png"))
        return false;

    sf::Texture& character02 = textures[Textures::Character02];
    if (!character02.loadFromFile(ressource_path_prefix + "data/character01.png"))
        return false;

    sf::Texture& character03 = textures[Textures::Character03];
    if (!character03.loadFromFile(ressource_path_prefix + "data/character01.png"))
        return false;

    sf::Texture& character04 = textures[Textures::Character04];
    if (!character04.loadFromFile(ressource_path_prefix + "data/character01.png"))
        return false;

    sf::Texture& character05 = textures[Textures::Character05];
    if (!character05.loadFromFile(ressource_path_prefix + "data/character01.png"))
        return false;

    sf::Texture& slime = textures[Textures::Slime];
    if (!slime.loadFromFile(ressource_path_prefix + "data/entities.png"))
        return false;

    return true;
}

bool RessourceManager::loadFont()
{
    if (!font.loadFromFile(ressource_path_prefix + "data/FSEX300.ttf"))
        return false;

    return true;
}

void compute_animation(TextureRect sprite_rect, sf::Vector2f start, int nb_frames,
                       std::vector<TextureRect>& frames)
{
    sf::Vector2f sprite_size = sprite_rect.end - sprite_rect.start;

    for (int i = 0; i < nb_frames; ++i)
    {
        TextureRect frame;
        frame.start = start;
        frame.end = start + sprite_size - sf::Vector2f(1.f, 1.f);

        frame.start.x += sprite_size.x * i;
        frame.end.x += sprite_size.x * i;
        frames.push_back(frame);
    }
}

bool RessourceManager::loadAnimations()
{
    std::ifstream animations_file(ressource_path_prefix + "data/animations");
    std::string character_type_str;

    if (!animations_file.is_open())
        return false;

    animations.clear();

    while (!animations_file.eof())
    {
        animations_file >> character_type_str;
        Class character_type;
        if (character_type_str == "Slime")
            character_type = Class::Slime;
        else if (character_type_str == "Knight")
            character_type = Class::Knight;
        else if (character_type_str == "Rogue")
            character_type = Class::Rogue;
        else if (character_type_str == "Wizard")
            character_type = Class::Wizard;

        EntityAnimationData animation_data;

        animations_file >> animation_data.sprite_rect.start.x;
        animations_file >> animation_data.sprite_rect.start.y;
        animations_file >> animation_data.sprite_rect.end.x;
        animations_file >> animation_data.sprite_rect.end.y;

        sf::Vector2f start;
        int nb_frames;

        animations_file >> start.x >> start.y >> nb_frames;
        compute_animation(animation_data.sprite_rect, start, nb_frames,
                          animation_data.animation[Direction::Down]);

        animations_file >> start.x >> start.y >> nb_frames;
        compute_animation(animation_data.sprite_rect, start, nb_frames,
                          animation_data.animation[Direction::Left]);

        animations_file >> start.x >> start.y >> nb_frames;
        compute_animation(animation_data.sprite_rect, start, nb_frames,
                          animation_data.animation[Direction::Right]);

        animations_file >> start.x >> start.y >> nb_frames;
        compute_animation(animation_data.sprite_rect, start, nb_frames,
                          animation_data.animation[Direction::Up]);

        animations[character_type] = animation_data;

        if (!animations_file)
            return false;

        animations_file >> std::ws;
    }

    return true;
}


const sf::Texture* RessourceManager::getTexture(Textures texture_type)
{
    auto it = textures.find(texture_type);
    if (it == textures.end())
        return nullptr;
    return &(it->second);
}

sf::Font& RessourceManager::getFont()
{
    return font;
}

TextureRect& RessourceManager::getSpriteRect(Class character_type)
{
    return animations[character_type].sprite_rect;
}

EntityAnimationData& RessourceManager::getAnimation(Class character_type)
{
    return animations[character_type];
}
