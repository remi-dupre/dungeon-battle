/**
 * \file config.hpp
 * \brief Manage configuration of the game.
 */

#pragma once

#include <string>
#include <cstdlib>

#include <SFML/Window.hpp>

/**
 * \brief Describe the general design of the level.
 */
enum class LevelType
{
    Flat, ///< Rectangular-shaped rooms
    Cave ///< Cave shaped rooms
};

/**
* \brief Parameters on how to generate the level
*/
struct GenerationMode
{
    bool infinite; ///< Wether the generation should or shouldn't give finite map
    int nb_rooms; ///< number of rooms to create on the map
    int room_min_size; ///< minimum number of cells contained on a room
    int room_max_size; ///< maximum number of cells contained on a room
    int room_margin; ///< minimum space added between two rooms

    float monster_load; ///< Number of monsters per 100 unit of space

    float maze_density; ///< Proportion of rooms that are replaced with mazes
    LevelType type; ///< Kind of design for the rooms
};

/**
 * \brief Represents the configuration of the game
 */
struct Configuration
{
    static const std::string data_path;  ///< Path containing static informations for the game
    static const std::string user_path;  ///< Path containing configuration and saves

    unsigned int width = 800;      ///< Width of the window, default value is 800
    unsigned int height = 600;     ///< Height of the window, default value is 600
    unsigned int scalefactor = 1;  ///< Scale factor for the window size, default value is 1
    bool fullscreen = false;       ///< Toggles fullscreen mode, default value is false
    bool vsync = true;             ///< Toggles vertical synchronisation, default value is true
    unsigned int maxfps = 60;      ///< Framerate limit of the window
    float animation_speed = 3.f;   ///< Animation speed
    bool lighting = true;          ///< Lighting enable or not
    bool monsters_no_delay = false; ///< Hero and monsters move at the same time

    sf::Keyboard::Key menu_key = sf::Keyboard::Key::Escape;   ///< Key pressed to display the menu
    sf::Keyboard::Key select_key = sf::Keyboard::Key::Return; ///< Key pressed to select an item
    sf::Keyboard::Key interaction_key = sf::Keyboard::Key::E; ///< Key pressed to interact with an entity
    sf::Keyboard::Key body_snatch_key = sf::Keyboard::Key::A; ///< Key pressed to interact with an entity

    sf::Keyboard::Key left_key = sf::Keyboard::Key::Q;  ///< Key pressed to go left
    sf::Keyboard::Key right_key = sf::Keyboard::Key::D; ///< Key pressed to go right
    sf::Keyboard::Key up_key = sf::Keyboard::Key::Z;    ///< Key pressed to go up
    sf::Keyboard::Key down_key = sf::Keyboard::Key::S;  ///< Key pressed to go down

    sf::Keyboard::Key attack_left_key = sf::Keyboard::Key::Left;   ///< Key pressed to attack an entity
    sf::Keyboard::Key attack_right_key = sf::Keyboard::Key::Right; ///< Key pressed to attack an entity
    sf::Keyboard::Key attack_up_key = sf::Keyboard::Key::Up;       ///< Key pressed to attack an entity
    sf::Keyboard::Key attack_down_key = sf::Keyboard::Key::Down;   ///< Key pressed to attack an entity

    // Generate a map
    GenerationMode gen_options;

    /**
     * \brief Default constructor
     *
     * This constructs a configuration structure with default values
     */
    Configuration();

    /**
     * \brief Read a configuration file
     * \param filename Path of the option file to load
     *
     * This function reads the configuration from a file.
     * It updates only the values found in the file, the other ones
     * are left unchanged.
     */
    void read(const std::string& filename);

    /**
     * \brief Read a game configuration file
     * \param filename Path of the option file to load
     *
     * This function reads the configuration from a file.
     * It updates only the values found in the file, the other ones
     * are left unchanged.
     */
    void readGame(const std::string& filename);

    /**
     * \brief Write a configuration file
     * \param filename Path of the option file to save
     *
     * This function saves the current configuration to a file.
     * All the options are saved, the old file is deleted.
     */
    void write(const std::string& filename) const;

    /**
     * \brief A configuration with default values
     */
    static const Configuration default_configuration;

    friend std::ostream& operator<<(std::ostream&, const Configuration&);
};

/**
 * \brief `operator<<` overload for `Configuration`
 *
 * Overloads `operator<<` to output configurations to streams
 */
std::ostream& operator<<(std::ostream& stream, const Configuration& config);
