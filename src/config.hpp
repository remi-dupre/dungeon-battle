#pragma once

#include <string>


/**
 * \brief The configuration
 */
struct Configuration
{
    unsigned int width, height;
    bool fullscreen;
    bool vsync;

    /**
     * \brief Parses a configuration file
     */
    void parse(const std::string& filename)
};
