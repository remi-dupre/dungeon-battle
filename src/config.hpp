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
    unsigned int width;      ///< Width of the window, default value is 800
    unsigned int height;     ///< Height of the window, default value is 600
    unsigned int scalefactor;///< Scale factor for the window size, default value is 1
    bool fullscreen;         ///< Toggles fullscreen mode, default value is off
    bool vsync;              ///< Toggles vertical synchronisation, default value is on


    /**
     * \brief Default constructor
     *
     * This construct a configuration structure with default values
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
};
