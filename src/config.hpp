/**
 * \file config.hpp
 * \brief Manage configuration of the game.
 */

#pragma once

#include <string>

#include <SFML/Window.hpp>


/**
 * \brief Represents the configuration of the game
 */
struct Configuration
{
    unsigned int width;      ///< Width of the window, default value is 800
    unsigned int height;     ///< Height of the window, default value is 600
    unsigned int scalefactor;///< Scale factor for the window size, default value is 1
    bool fullscreen;         ///< Toggles fullscreen mode, default value is false
    bool vsync;              ///< Toggles vertical synchronisation, default value is true
    unsigned int maxfps;     ///< Framerate limit of the window

    sf::Keyboard::Key left_key;        ///< Key pressed to go left, default value is sf::Keyboard::Q
    sf::Keyboard::Key right_key;       ///< Key pressed to go right, default value is sf::Keyboard::D
    sf::Keyboard::Key up_key;          ///< Key pressed to go up, default value is sf::Keyboard::Z
    sf::Keyboard::Key down_key;        ///< Key pressed to go down, default value is sf::Keyboars::S
    sf::Keyboard::Key menu_key;        ///< Key pressed to display the menu, default value is sf::Keyboard::Escape

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
     * are not changed.
     */
    void read(const std::string& filename);

    /**
     * \brief Write a configuration file
     * \param filename Path of the option file to save
     *
     * This function saves the current configuration to a file.
     * All the options are saved, the old file is deleted.
     */
    void write(const std::string& filename);

    static const Configuration default_configuration;
};
