/**
 * \file math.hpp
 * \brief Helper math functions.
 */

#pragma once

#include <cmath>

#include <SFML/System/Vector2.hpp>


/**
 * \brief Basical math operations.
 */
namespace math
{
    template <typename T>
    constexpr T pi = 3.14159265358979323846264338327950; ///< The constant pi

    template <typename T>
    constexpr T two_pi = 2.0 * math::pi<T>; ///< The constant 2 * pi

    /**
     * \brief Compute the dot product of two 2D vectors
     * \param a The first 2D vector
     * \param b The second 2D vector
     * \return The dot product of *a* and *b*
     *
     * This function computes the dot product of two `sf::Vector2`.
     */
    template <typename T>
    constexpr T dot(const sf::Vector2<T> a, const sf::Vector2<T> b);

    /**
     * \brief Compute the Hadamard product of two 2D vectors
     * \param a A 2D vector
     * \param b A 2D vector
     * \return The Hadamard product of *a* and *b*
     *
     * This function computes the Hadamard product of two `sf::Vector2`.
     */
    template <typename T>
    constexpr sf::Vector2<T> hadamard(const sf::Vector2<T> a, const sf::Vector2<T> b);

    /**
     * \brief Compute the cross product of two 2D vectors
     * \param a A 2D vector
     * \param b A 2D vector
     * \return The third coordinate of the cross product of *a* and *b*
     *
     * This function computes the third coordinate of the cross product of two `sf::Vector2`.
     */
    template <typename T>
    constexpr T cross(const sf::Vector2<T> a, const sf::Vector2<T> b);

    /**
     * \brief Compute the distance between two 2D vectors
     * \param a The first 2D vector
     * \param b The second 2D vector
     * \return The distance between *a* and *b*
     *
     * This function computes the distance between two `sf::Vector2`.
     */
    template <typename T>
    constexpr T distance(const sf::Vector2<T> a, const sf::Vector2<T> b);

    /**
     * \brief Compute the length squared of a vector
     * \param a A 2D vector
     * \return The length squared of *a*
     *
     * This function computes the length squared of a `sf::Vector2`.
     */
    template <typename T>
    constexpr T length_sqr(const sf::Vector2<T> a);

    /**
     * \brief Compute the length of a vector
     * \param a A 2D vector
     * \return The length of *a*
     *
     * This function computes the length of a `sf::Vector2`.
     */
    template <typename T>
    constexpr T length(const sf::Vector2<T> a);

    /**
     * \brief Normalize a 2D vector
     * \param a A 2D vector
     * \return The *a* vector normalized
     *
     * This function normalizes a `sf::vector2`.
     */
    template <typename T>
    constexpr sf::Vector2<T> normalize(const sf::Vector2<T> a);

    /**
     * \brief Round the coordinates of a 2D vector
     * \param a A 2D vector
     * \return The vector with coordinates rounded down
     */
    template <typename T>
    constexpr sf::Vector2<T> floor(const sf::Vector2<T> a);

    /**
     * \brief Round up the coordinates of a 2D vector
     * \param a A 2D vector
     * \return The vector with coordinates rounded up
     */
    template <typename T>
    constexpr sf::Vector2<T> ceil(const sf::Vector2<T> a);

    /**
     * \brief Divide the coordinates of a 2D int vector with rounding down
     * \param a A 2D vector
     * \param b An integer
     * \return The vector with coordinates divided
     */
    sf::Vector2i divide_floor(sf::Vector2i a, int b);

    /**
     * \brief Compute the remainder of vector division
     * \param a A 2D vector
     * \param b An integer
     * \return The remainder of vector
     */
    sf::Vector2i remainder(sf::Vector2i a, int b);

    /**
     * \brief Compute the distance un norm 1 between two 2D vectors
     * \param a The first 2D vector
     * \param b The second 2D vector
     * \return The distance in norm 1 between *a* and *b*
     *
     * This function computes the distance between two `sf::Vector2`.
     */
    template <typename T>
    constexpr T distance_1(const sf::Vector2<T> a, const sf::Vector2<T> b);

    #include "math.inl"
}
