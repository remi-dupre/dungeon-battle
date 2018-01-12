#include <fstream>
#include <string>
#include <vector>

// For mkdir
#include <cstdlib>

#include "game.hpp"


bool save_dungeon(const std::vector<Level>& dungeon,
                  const std::vector<MapExploration>& exploration,
                  const std::vector<std::shared_ptr<Generator>>& generators,
                  const std::string& save_path);

bool Game::saveGame()
{
    std::cout << "Saving game" << std::endl;

    const std::string save_path {Configuration::user_path + "saves/" + game_name + "/"};

    if (system(("mkdir -p " + save_path + "levels").c_str()) == -1)
        return false;

    // General informations about the game.
    std::ofstream save_file {save_path + "game.save", std::ios::trunc | std::ios::binary};
    if (!save_file)
        return false;

    save_file << game_name << "\n"; // game's name

    unsigned char turn = entity_turn == EntityType::Hero ? 1 : 0;
    save_file.write(reinterpret_cast<char*>(&turn), sizeof(turn)); // entity_turn
    uint32_t level = current_level;
    save_file.write(reinterpret_cast<char*>(&level), sizeof(level)); // current level
    level = dungeon.size();
    save_file.write(reinterpret_cast<char*>(&level), sizeof(level)); // max level

    save_file.close();

    save_dungeon(dungeon, exploration, generators, save_path);

    std::fstream saves {Configuration::user_path + "saves/saves.data"};
    if (!saves)
        return false;
    std::vector<std::string> save_names;
    std::string name;

    while (saves >> name >> std::ws)
        save_names.push_back(name);
    saves.clear();
    saves.seekp(0, std::ios::end);
    auto it = std::find(save_names.begin(), save_names.end(), game_name);
    if (it == save_names.end())
        saves << name << std::endl;
    saves.close();

    return true;
}

bool save_dungeon(const std::vector<Level>& dungeon,
                  const std::vector<MapExploration>& exploration,
                  const std::vector<std::shared_ptr<Generator>>& generators,
                  const std::string& save_path)
{
    for (uint32_t i_level = 0; i_level < dungeon.size(); ++i_level)
    {
        const Level& level = dungeon[i_level];
        level.map.saveToFile(save_path + "levels/map" + std::to_string(i_level) + ".dat");

        exploration[i_level].save(save_path + "levels/exploration" + std::to_string(i_level) + ".dat");

        std::ofstream entities_file {
            save_path + "levels/entities" + std::to_string(i_level) + ".dat",
            std::ios::trunc | std::ios::binary
        };
        uint32_t n_entities = level.entities.size();
        entities_file.write(reinterpret_cast<char*>(&n_entities), sizeof(uint32_t));
        for (const auto& entity : level.entities)
            entities_file << entity;

        std::ofstream generator_file {
            save_path + "levels/generator" + std::to_string(i_level) + ".dat",
            std::ios::trunc | std::ios::binary
        };
        generator_file << *generators[i_level];
    }

    return true;
}
