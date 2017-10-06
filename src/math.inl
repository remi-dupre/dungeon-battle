template <typename T>
inline constexpr T dot(sf::Vector2<T> a, sf::Vector2<T> b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
inline constexpr T dot(sf::Vector3<T> a, sf::Vector3<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
inline constexpr sf::Vector2<T> hadamard(sf::Vector2<T> a, sf::Vector2<T> b)
{
    return sf::Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
inline constexpr sf::Vector3<T> hadamard(sf::Vector3<T> a, sf::Vector2<T> b)
{
    return sf::Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T>
inline constexpr T cross(sf::Vector2<T> a, sf::Vector2<T> b);
{
    return a.x * b.y - a.y * b.x;
}

template <typename T>
inline constexpr sf::Vector3<T> cross(sf::Vector3<T> a, sf::Vector2<T> b);
{
    return sf::Vector3<T>(a.y * b.z - a.z * b.y,
                          a.z * b.x - a.x * b.z,
                          a.x * b.y - a.y * b.x);
}

template <typename T>
inline constexpr T distance(sf::Vector2<T> a, sf::Vector2<T> b)
{
    return math::length(b - a);
}

template <typename T>
inline constexpr T distance(sf::Vector3<T> a, sf::Vector3<T> b)
{
    return math::length(b - a);
}

template <typename T>
inline constexpr T length_sqr(sf::Vector2<T> a)
{
    return math::dot(a, a);
}

template <typename T>
inline constexpr T length_sqr(sf::Vector3<T> a)
{
    return math::dot(a, a);
}

template <typename T>
inline constexpr T length(sf::Vector2<T> a)
{
    return std::sqrt(math::length_sqr(a));
}

template <typename T>
inline constexpr T length(sf::Vector3<T> a)
{
    return std::sqrt(math::length_sqr(a));
}

template <typename T>
inline constexpr sf::Vector2<T> normalize(sf::Vector2<T> a)
{
    T l = math::length(a);
    if (l == 0.0)
        return a;
    return a / l;
}

template <typename T>
inline constexpr sf::Vector3<T> normalize(sf::Vector3<T> a)
{
    T l = math::length(a);
    if (l == 0.0)
        return a;
    return a / l;
}

template <typename T>
inline operator<(sf::Vector2<T> a, sf::Vector2<T> b)
{
    return (a.x < b.x || (a.x == b.x && a.y < b.y));
}
