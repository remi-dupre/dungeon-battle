/**
 * \file utility.hpp
 * \brief Various definitions used in different modules.
 */

#pragma once

#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>


/**
 * \brief   Give a sequence of coordinates covering a square, starting from the center and describing a spiral.
 * \param   x  Center of the square.
 * \param   y  Center of the square.
 * \param   radius  Radius of the square.
 * \return  The cells ordered from center to extremity.
 */
std::vector<std::pair<int, int>> spiral(int x, int y, int radius);

/**
 * \brief Direction of an entity or an action
 */
enum class Direction : uint8_t
{
    None  = 0,      ///< No specific direction
    Up    = 1 << 0, ///< Up
    Right = 1 << 1, ///< Right
    Down  = 1 << 2, ///< Down
    Left  = 1 << 3  ///< Left
};

Direction operator|(Direction a, Direction b);
Direction operator&(Direction a, Direction b);
Direction operator|=(Direction& a, Direction b);
Direction operator&=(Direction& a, Direction b);
bool has_direction(Direction a, Direction b);

sf::Vector2i to_vector2i(Direction direction);

constexpr Direction directions[] = {Direction::Left, Direction::Up, Direction::Right, Direction::Down};

namespace vec
{
    template <typename T>
    sf::Vector2<T> position(sf::Rect<T> a)
    {
        return {a.left, a.top};
    }

    template <typename T>
    sf::Vector2<T> size(sf::Rect<T> a)
    {
        return {a.width, a.height};
    }
}

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
     * \brief Serialisation of a pair.
     */
    std::ostream& operator<<(std::ostream& stream, const std::pair<int, int>& pair);

    /**
     * \brief Unserialisation of a pair.
     */
    std::istream& operator>>(std::istream& stream, std::pair<int, int>& pair);

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
