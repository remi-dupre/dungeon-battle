#include <fstream>

#include "game.hpp"


bool load_dungeon(uint32_t dungeon_size,
                  std::vector<Level>& dungeon,
                  std::vector<MapExploration>& exploration,
                  std::string& load_path);

bool Game::loadGame(const std::string& save_name)
{
    dungeon.clear();
    exploration.clear();
    generators.clear();

    current_level = 0;
    map = nullptr;
    map_exploration = nullptr;
    entities = nullptr;

    std::string load_path {Configuration::user_path + "saves/" + save_name + "/"};

    // General informations about the game.
    std::ifstream save_file {load_path + "game.save", std::ios::binary};
    if (!save_file)
        return false;

    save_file >> game_name >> std::ws; // game's name

    uint8_t turn;;
    save_file.read(reinterpret_cast<char*>(&turn), sizeof(uint8_t)); // entity_turn
    if (turn == 0)
        entity_turn = EntityType::Monster;
    else
        entity_turn = EntityType::Hero;

    save_file.read(reinterpret_cast<char*>(&current_level), sizeof(uint32_t)); // current level

    uint32_t max_level {};
    save_file.read(reinterpret_cast<char*>(&max_level), sizeof(uint32_t)); // max level

    // Fake all generators
    generators.resize(max_level, new Generator());

    load_dungeon(max_level, dungeon, exploration, load_path);

    map = &dungeon[current_level].map;
    generator = generators[current_level];
    entities = &dungeon[current_level].entities;
    map_exploration = &exploration[current_level];

    return true;
}


bool load_dungeon(uint32_t dungeon_size,
                  std::vector<Level>& dungeon,
                  std::vector<MapExploration>& exploration,
                  std::string& save_path)
{
    for (uint32_t i_level = 0; i_level < dungeon_size; ++i_level)
    {
        dungeon.emplace_back();
        exploration.emplace_back();
        Level& level = dungeon.back();
        MapExploration& map_exploration = exploration.back();

        level.map.loadFromFile(save_path + "levels/map" + std::to_string(i_level) + ".dat");
        map_exploration.load(save_path + "levels/exploration" + std::to_string(i_level) + ".dat");

        std::ifstream entities_file {
            save_path + "levels/entities" + std::to_string(i_level) + ".dat",
            std::ios::binary
        };
        uint32_t n_entities {0};
        entities_file.read(reinterpret_cast<char*>(&n_entities), sizeof(uint32_t));
        for (unsigned int i {0}; i < n_entities; ++i)
        {
            EntityType type {EntityType::None};
            entities_file.read(reinterpret_cast<char*>(&type), sizeof(uint32_t));
            entities_file.seekg(-static_cast<int>(sizeof(uint32_t)), std::ios::cur);
            std::shared_ptr<Entity> entity {nullptr};
            if (type == EntityType::Hero || type == EntityType::Monster)
            {
                entity = std::make_shared<Character>();
                entities_file >> (*std::static_pointer_cast<Character>(entity));
            }
            // else if (type == EntityType::Item)
            // {
            //     entity = std::make_shared<Item>();
            //     entities_file >> (*std::static_pointer_cast<Item>(entity));
            // }
            else
            {
                entity = std::make_shared<Entity>();
                entities_file >> (*entity);
            }
            level.entities.push_back(entity);
        }
    }

    return true;
}
