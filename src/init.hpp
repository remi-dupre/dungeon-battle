/**
 * \file init.hpp
 * \brief Initialization functions.
 */

#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "config.hpp"


/**
 * \brief Initialization functions.
 */
namespace init
{
    /**
     * \brief Initialize a new configuration
     * \param filename Path of the option file to load
     * \return A Configuration object initialized with filename
     *
     * This function reads the configuration from a file.
     * It updates only the values found in the file, the other ones
     * are not changed.
     */
    Configuration configuration(const std::string& filename);

    /**
     * \brief Initialize a new window
     * \param config Configuration which is used to initialize the window
     * \return A pointer to a RenderWindow object initialized from config
     *
     * This function creates a RenderWindow from a configuration.
     * It retrieves all the relevant informations from config and
     * returns a pointer to the window.
     */
    sf::RenderWindow* window(const Configuration& config);
}