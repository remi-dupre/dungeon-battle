#pragma once

#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

#include "entity.hpp"


using namespace std::literals::string_literals;

enum class Textures
{
    Tileset,
    Scenery,

    Character01,
    Character02,
    Character03,
    Character04,
    Character05,

    Slime
};

struct TextureRect
{
    TextureRect() = default;

    sf::Vector2f start;
    sf::Vector2f end;
};

struct EntityAnimationData
{
    EntityAnimationData() = default;

    TextureRect getFrame(Direction dir, float frame_ratio);

    TextureRect sprite_rect;
    std::map<Direction, std::vector<TextureRect>> animation;
};

class RessourceManager
{
public:

    static bool loadRessources();

    static const sf::Texture* getTexture(Textures texture_type);
    static sf::Font& getFont();

    static TextureRect& getSpriteRect(Class character_type);
    static EntityAnimationData& getAnimation(Class character_type);

private:

    static bool loadTextures();
    static bool loadFont();

    static bool loadAnimations();

    static std::map<Textures, sf::Texture> textures;
    static sf::Font font;

    static std::map<Class, EntityAnimationData> animations;

    static const std::string ressource_path_prefix;
};
