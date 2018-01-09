#include "utility.hpp"


Direction operator|(Direction a, Direction b)
{
    return static_cast<Direction>(static_cast<int>(a) | static_cast<int>(b));
}

Direction operator&(Direction a, Direction b)
{
    return static_cast<Direction>(static_cast<int>(a) & static_cast<int>(b));
}

Direction operator|=(Direction& a, Direction b)
{
    return a = a | b;
}

Direction operator&=(Direction& a, Direction b)
{
    return a = a & b;
}

bool has_direction(Direction a, Direction b)
{
    return static_cast<int>(a & b) != 0;
}

sf::Vector2i to_vector2i(Direction direction)
{
    sf::Vector2i result = {0, 0};

    if (has_direction(direction, Direction::Down))
        result.y += 1;
    if (has_direction(direction, Direction::Up))
        result.y -= 1;
    if (has_direction(direction, Direction::Right))
        result.x += 1;
    if (has_direction(direction, Direction::Left))
        result.x -= 1;

    return result;
}

std::pair<int, int> std::operator +(const std::pair<int, int>& x, const std::pair<int, int>& y) {
    return std::make_pair(x.first + y.first, x.second + y.second);
}

std::pair<int, int> std::operator -(const std::pair<int, int>& x, const std::pair<int, int>& y) {
    return std::make_pair(x.first - y.first, x.second - y.second);
}

void std::operator +=(std::pair<int, int>& x, const std::pair<int, int>& y) {
    x = x + y;
}

void std::operator -=(std::pair<int, int>& x, const std::pair<int, int>& y) {
    x = x - y;
}

std::pair<int, int> std::operator %(const std::pair<int, int>& x, int n) {
    assert(n > 0);

    return std::make_pair((x.first % n + n) % n, (x.second % n + n) % n);
}

std::pair<int, int> std::operator /(const std::pair<int, int>& x, int n) {
    assert(n > 0);

    return std::make_pair(x.first / n, x.second / n);
}
