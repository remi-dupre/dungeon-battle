/**
 * \file args.hpp
 * \brief Parse command line arguments.
 */

#pragma once

#include <map>
#include <string>


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

/*
 * \brief Helper function that return the length of a C-style array
 * \param T A C-style array (a pointer)
 * \return The length of T
 */
template <typename T, int N>
inline constexpr std::size_t array_length(T (&)[N])
{
    return N;
}
