/**
 * \file utility.hpp
 * \brief Various definitions used in different modules.
 */

#pragma once

#include <functional>
#include <tuple>

#include <SFML/System/Vector2.hpp>


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

    /**
     * \brief Get the sum of two pairs.
     * \param x A pair of int.
     * \param y A pair of int.
     * \return x+y, the vector obtained by summing up both pairs.
     */
    std::pair<int, int> operator +(const std::pair<int, int>& x, const std::pair<int, int>& y);

    /**
     * \brief Get the difference of two pairs.
     * \param x A pair of int.
     * \param y A pair of int.
     * \return x-y, the vector obtained by substracting both pairs.
     */
    std::pair<int, int> operator -(const std::pair<int, int>& x, const std::pair<int, int>& y);

    /**
     * \brief Add a pair to another.
     * \param x A pair of int that will be incremented.
     * \param y A pair of int to add to x.
     */
    void operator +=(std::pair<int, int>& x, const std::pair<int, int>& y);

    /**
     * \brief Remove a pair from another.
     * \param x A pair of int that will be decremented.
     * \param y A pair of int to remove from x.
     */
    void operator -=(std::pair<int, int>& x, const std::pair<int, int>& y);
}
