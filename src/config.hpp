/**
 * \file config.hpp
 * \brief Manage configuration.
 */

#pragma once

#include <string>


/**
 * \brief Represent the configuration of the game
 */
struct Configuration
{
    unsigned int width; ///< Width of the window
    unsigned int height; ///< Height of the window
    bool fullscreen; ///< Toggles fullscreen mode
    bool vsync; ///< Toggles vertical synchronisation


    Configuration();

    /**
     * \brief Parses a configuration file
     * \param filename Path of the option file to load
     *
     * Load the configuration from a file
     */
    void parse(const std::string& filename);
};
