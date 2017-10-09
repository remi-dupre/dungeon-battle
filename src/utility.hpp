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
    /**
     * \brief Specialisation of std::less for sf::Vector2i
     */
    template<> struct less<sf::Vector2i>
    {
        bool operator() (const sf::Vector2i& a, const sf::Vector2i& b) const
       {
           return a.x < b.x || (a.x == b.x && a.y < b.y);
       }
    };

    /**
     * \brief Specialisation of hash struct for sf::Vector2i
     */
    template<> struct hash<sf::Vector2i>
    {
        size_t operator()(const sf::Vector2i& v) const
        {
            return 73856093 * v.x ^ 83492791 * v.y;
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
}
