/**
 * \file utility.hpp
 * \brief Various definitions used in different modules.
 */

#pragma once

#include <cassert>
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

sf::Vector2i to_vector2i(Direction direction);

namespace std
{
    /**
     * \brief Specialisation of std::less for sf::Vector2<T>
     */
    template<typename T>
    struct less<sf::Vector2<T>>
    {
        bool operator() (const sf::Vector2<T>& a, const sf::Vector2<T>& b) const
       {
           return a.x < b.x || (a.x == b.x && a.y < b.y);
       }
    };

    /**
     * \brief Specialisation of std::hash for sf::Vector2<T>
     */
    template <typename T>
    struct hash<sf::Vector2<T>>
    {
        size_t operator()(const sf::Vector2<T>& v) const
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

    /**
     * \brief Remove a pair from another.
     * \param x A pair of int that will be decremented.
     * \param y A pair of int to remove from x.
     */
    void operator -=(std::pair<int, int>& x, const std::pair<int, int>& y);

    /**
     * \brief Get the quotient of a pair divided by a positive integer.
     * \param x A pair of int.
     * \param n A positive int.
     * \return x / n, the vector obtained by processing coordinates divided by n.
     */
    std::pair<int, int> operator /(const std::pair<int, int>& x, int n);

    /**
     * \brief Get the postitive modulo of a pair.
     * \param x A pair of int.
     * \param n A positive int.
     * \return x % n, the vector obtained by processing coordinates modulo n.
     */
    std::pair<int, int> operator %(const std::pair<int, int>& x, int n);
}
