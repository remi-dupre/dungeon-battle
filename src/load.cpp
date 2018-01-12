#include <fstream>

#include "game.hpp"


bool load_dungeon(uint32_t dungeon_size,
                  std::vector<Level>& dungeon,
                  std::vector<MapExploration>& exploration,
                  std::vector<std::shared_ptr<Generator>>& generators,
                  std::string& load_path,
                  const GenerationMode& parameters);

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

    load_dungeon(max_level, dungeon, exploration, generators, load_path, config.gen_options);

    map = &dungeon[current_level].map;
    generator = generators[current_level];
    entities = &dungeon[current_level].entities;
    map_exploration = &exploration[current_level];

    return true;
}


bool load_dungeon(uint32_t dungeon_size,
                  std::vector<Level>& dungeon,
                  std::vector<MapExploration>& exploration,
                  std::vector<std::shared_ptr<Generator>>& generators,
                  std::string& save_path,
                  const GenerationMode& parameters)
{
    for (uint32_t i_level = 0; i_level < dungeon_size; ++i_level)
    {
        dungeon.emplace_back();
        generators.push_back(std::make_shared<Generator>(parameters));
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
        level.entities.resize(n_entities);
        for (unsigned int i {0}; i < n_entities; ++i)
            entities_file >> level.entities[i];

        std::ifstream generator_file {
            save_path + "levels/generator" + std::to_string(i_level) + ".dat",
            std::ios::binary
        };
        generator_file >> *generators[i_level];
    }

    return true;
}
