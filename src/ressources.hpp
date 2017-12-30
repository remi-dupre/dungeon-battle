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

    static bool loadRessources(const std::string& ressources_path);

    static bool loadTextures();
    static bool loadFont();

    static bool loadAnimations();

    static void setRessourcesPath(const std::string& ressources_path_)
    { ressources_path = ressources_path_; };


    static sf::Texture& getTexture(Textures texture_type);
    static sf::Font& getFont();

    static sf::IntRect getSpriteRect(EntitySprite entity_type);
    static EntityAnimationData& getAnimation(EntitySprite entity_type);

private:

    static std::string ressources_path;

    static std::map<Textures, sf::Texture> textures;
    static std::map<EntitySprite, EntityAnimationData> animations;
    static sf::Font font;
};

namespace vec
{
    template <typename T>
    sf::Vector2<T> position(sf::Rect<T> a)
    {
        return {a.left, a.top};
    }

    template <typename T>
    sf::Vector2<T> size(sf::Rect<T> a)
    {
        return {a.width, a.height};
    }
}
