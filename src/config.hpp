/**
 * \file config.hpp
 * \brief Manage configuration.
 */

#pragma once

#include <string>


/**
 * \brief The configuration
 */
struct Configuration
{
    unsigned int width; ///< Width of the window
    unsigned int height; ///< Height of the window
    bool fullscreen; ///< Toggles fullscreen mode
    bool vsync; ///< Toggles vertical synchronisation

    /**
     * \brief Parses a configuration file
     * \param filename Path to the configuration file
     */
    void parse(const std::string& filename);
};
