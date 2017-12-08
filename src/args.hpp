/**
 * \file args.hpp
 * \brief Parse command line arguments.
 */

#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <tuple>


/**
 * \brief Class representing the differents options available
 */
enum class Option
{
    Help, ///< Print help
    Config, ///< Change configuration file
};

/**
 * \brief Parses command line arguments
 * \param options Map that will store the values of arguments
 * \param argc Length of argv
 * \param argv Arguments passed to the executable
 *
 * This function parses argv starting at index 1.
 * It updates the values in options according to the arguments
 * stored in argv.
 */
int parse_arguments(std::map<Option, std::string>& options, int argc, char** argv);


template <typename T, int N>
inline constexpr std::size_t array_length(T (&)[N])
{
    return N;
}
