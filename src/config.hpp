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
     * \param filename The file to load the configuration from
     *
     * Parses the file filename and update the options of the configuration accordingly
     */
    void parse(const std::string& filename);
};
