#pragma once

#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

#include "utility.hpp"


using namespace std::literals::string_literals;

enum class Textures
{
    Tileset,
    Scenery,

    Knight,
    Rogue,
    Wizard,
    Character04,
    Character05,

    Slime = Scenery
};

enum class EntitySprite
{
    StairsUp,
    StairsDown,

    Knight,
    Rogue,
    Wizard,

    Slime,

    None
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

    sf::IntRect getFrame(Direction dir, float frame_ratio);

    sf::IntRect sprite_rect;
    std::map<Direction, std::vector<sf::IntRect>> animation;
};

class RessourceManager
{
public:

    static bool loadRessources();

    static sf::Texture& getTexture(Textures texture_type);
    static sf::Font& getFont();

    static sf::IntRect getSpriteRect(EntitySprite character_type);
    static EntityAnimationData& getAnimation(EntitySprite character_type);

private:

    static bool loadTextures();
    static bool loadFont();

    static bool loadAnimations();

    static std::map<Textures, sf::Texture> textures;
    static sf::Font font;

    static std::map<EntitySprite, EntityAnimationData> animations;

    static const std::string ressource_path_prefix;
};

namespace vec
{
    template <typename T>
    sf::Vector2<T> position(sf::Rect<T> a)
    {
        return {a.left, a.top};
    };

    template <typename T>
    sf::Vector2<T> size(sf::Rect<T> a)
    {
        return {a.width, a.height};
    };
}
