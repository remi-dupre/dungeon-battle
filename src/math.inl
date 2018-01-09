template <typename T>
inline constexpr T dot(const sf::Vector2<T> a, const sf::Vector2<T> b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
inline constexpr sf::Vector2<T> hadamard(const sf::Vector2<T> a, const sf::Vector2<T> b)
{
    return sf::Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
inline constexpr T cross(const sf::Vector2<T> a, const sf::Vector2<T> b)
{
    return a.x * b.y - a.y * b.x;
}

template <typename T>
inline constexpr T distance(const sf::Vector2<T> a, const sf::Vector2<T> b)
{
    return math::length(b - a);
}

template <typename T>
inline constexpr T length_sqr(const sf::Vector2<T> a)
{
    return math::dot(a, a);
}

template <typename T>
inline constexpr T length(const sf::Vector2<T> a)
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
inline constexpr sf::Vector2<T> floor(const sf::Vector2<T> a)
{
    return {std::floor(a.x), std::floor(a.y)};
}

template <typename T>
inline constexpr sf::Vector2<T> ceil(const sf::Vector2<T> a)
{
    return {std::ceil(a.x), std::ceil(a.y)};
}

inline sf::Vector2i divide_floor(sf::Vector2i a, int b)
{
    return static_cast<sf::Vector2i>
        (math::floor(static_cast<sf::Vector2f>(a) / static_cast<float>(b)));
}

inline sf::Vector2i remainder(sf::Vector2i a, int b)
{
    return a - math::divide_floor(a, b) * b;
}


template <typename T>
inline constexpr T distance_1(const sf::Vector2<T> a,const sf::Vector2<T> b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
