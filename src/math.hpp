/**
 * \file math.hpp
 * \brief Helper math functions.
 */

#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>


namespace math
{
    template <typename T>
    constexpr T pi = 3.14159265358979323846264338327950; ///< The constant pi

    template <typename T>
    constexpr T two_pi = 2.0 * math::pi<T>; ///< The constant 2 * pi

    /**
     * \Brief Compute the dot product of two 2D vectors
     * \param a The first 2D vector
     * \param b The second 2D vector
     * \return The dot product of a and b
     *
     * This function computes the dot product of two `sf::Vector2`.
     */
    template <typename T>
    inline constexpr T dot(sf::Vector2<T> a, sf::Vector2<T> b)
    {
        return a.x * b.x + a.y * b.y;
    };

    /**
     * \Brief Compute the dot product of two 3D vectors
     * \param a The first 3D vector
     * \param b The second 3D vector
     * \return The dot product of a and b
     *
     * This function computes the dot product of two `sf::Vector3`.
     */
    template <typename T>
    inline constexpr T dot(sf::Vector3<T> a, sf::Vector3<T> b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    };

    /**
     * \Brief Compute the length squared of a vector
     * \param a A 2D vector
     * \return The length squared of a
     *
     * This function computes the length squared of a `sf::Vector2`.
     */
    template <typename T>
    inline constexpr T length_sqr(sf::Vector2<T> a)
    {
        return math::dot(a, a);
    };

    /**
     * \Brief Compute the length squared of a vector
     * \param a A 3D vector
     * \return The length squared of a
     *
     * This function computes the length squared of a `sf::Vector3`.
     */
    template <typename T>
    inline constexpr T length_sqr(sf::Vector3<T> a)
    {
        return math::dot(a, a);
    };

    /**
     * \Brief Compute the length of a vector
     * \param a A 2D vector
     * \return The length of a
     *
     * This function computes the length of a `sf::Vector2`.
     */
    template <typename T>
    inline constexpr T length(sf::Vector2<T> a)
    {
        return std::sqrt(math::length_sqr(a));
    };

    /**
     * \Brief Compute the length of a vector
     * \param a A 3D vector
     * \return The length of a
     *
     * This function computes the length of a `sf::Vector3`.
     */
    template <typename T>
    inline constexpr T length(sf::Vector3<T> a)
    {
        return std::sqrt(math::length_sqr(a));
    };

    /**
     * \Brief Normalize a 2D vector
     * \param a A 2D vector
     * \return The vector normalized
     *
     * This function normalizes a `sf::vector2`.
     */
    template <typename T>
    inline constexpr sf::Vector2<T> normalize(sf::Vector2<T> a)
    {
        T l = math::length(a);
        if (l == 0.0)
            return a;
        return a / l;
    };

    /**
     * \Brief Normalize a 3D vector
     * \param a A 3D vector
     * \return The vector normalized
     *
     * This function normalizes a `sf::vector3`.
     */
    template <typename T>
    inline constexpr sf::Vector3<T> normalize(sf::Vector3<T> a)
    {
        T l = math::length(a);
        if (l == 0.0)
            return a;
        return a / l;
    };
}
