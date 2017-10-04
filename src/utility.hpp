/**
 * \file utility.hpp
 * \brief Various definitions used in different modules.
 */

#pragma once

#include <functional>


/**
 * \brief Direction of an entity or an action
 */
enum class Direction
{
    None, ///< No specific direction
    Left, ///< Left
    Right, ///< Right
    Up, ///< Up
    Down ///< Down
};

namespace std
{
    template<> struct hash<sf::Vector2i>
    {
        size_t operator()(const sf::Vector2i& v) const
        {
            return 349 * v.x + 547 * v.y;
        }
    };
}
