#pragma once

#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

#include "map.hpp"
#include "utility.hpp"


using namespace std::literals::string_literals;

enum class Textures
{
    Tileset,
    Scenery,

    Warrior,
    Rogue  ,
    Wizard ,
    Angel  ,

    Goat,

    Bat,
    Slime = Scenery
};

enum class EntitySprite
{
    StairsUp,
    StairsDown,

    Warrior,
    Rogue  ,
    Wizard ,
    Angel  ,

    Goat,

    Slime,
    Bat,

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


    static sf::Vector2f getTileTextureCoords(CellType cell_type, Direction neighborhood);

    static sf::Texture& getTexture(Textures texture_type);
    static sf::Font& getFont();

    static sf::IntRect getSpriteRect(EntitySprite entity_type);
    static EntityAnimationData& getAnimation(EntitySprite entity_type);

private:

    static std::string ressources_path;

    static const sf::Vector2f ground_texture_coords[];
    static const sf::Vector2f wall_texture_coords[];

    static std::map<Textures, sf::Texture> textures;
    static std::map<EntitySprite, EntityAnimationData> animations;
    static sf::Font font;
};
