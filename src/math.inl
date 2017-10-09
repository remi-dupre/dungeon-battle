template <typename T>
inline constexpr T dot(const sf::Vector2<T> a, const sf::Vector2<T> b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
inline constexpr T dot(const sf::Vector3<T> a, const sf::Vector3<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
inline constexpr sf::Vector2<T> hadamard(const sf::Vector2<T> a, const sf::Vector2<T> b)
{
    return sf::Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
inline constexpr sf::Vector3<T> hadamard(const sf::Vector3<T> a, const sf::Vector2<T> b)
{
    return sf::Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T>
inline constexpr T cross(const sf::Vector2<T> a, const sf::Vector2<T> b)
{
    return a.x * b.y - a.y * b.x;
}

template <typename T>
inline constexpr sf::Vector3<T> cross(const sf::Vector3<T> a, const sf::Vector2<T> b)
{
    return sf::Vector3<T>(a.y * b.z - a.z * b.y,
                          a.z * b.x - a.x * b.z,
                          a.x * b.y - a.y * b.x);
}

template <typename T>
inline constexpr T distance(const sf::Vector2<T> a, const sf::Vector2<T> b)
{
    return math::length(b - a);
}

template <typename T>
inline constexpr T distance(const sf::Vector3<T> a, const sf::Vector3<T> b)
{
    return math::length(b - a);
}

template <typename T>
inline constexpr T length_sqr(const sf::Vector2<T> a)
{
    return math::dot(a, a);
}

template <typename T>
inline constexpr T length_sqr(const sf::Vector3<T> a)
{
    return math::dot(a, a);
}

template <typename T>
inline constexpr T length(const sf::Vector2<T> a)
{
    return std::sqrt(math::length_sqr(a));
}

template <typename T>
inline constexpr T length(const sf::Vector3<T> a)
{
    return std::sqrt(math::length_sqr(a));
}

template <typename T>
inline constexpr sf::Vector2<T> normalize(const sf::Vector2<T> a)
{
    T l = math::length(a);
    if (l == 0.0)
        return a;
    return a / l;
}

template <typename T>
inline constexpr sf::Vector3<T> normalize(const sf::Vector3<T> a)
{
    T l = math::length(a);
    if (l == 0.0)
        return a;
    return a / l;
}

template <typename T>
inline constexpr sf::Vector2<T> floor(const sf::Vector2<T> a)
{
    return {std::floor(a.x), std::floor(a.y)};
}

template <typename T>
inline constexpr sf::Vector2<T> ceil(const sf::Vector2<T> a)
{
    return {std::ceil(a.x), std::ceil(a.y)};
}
