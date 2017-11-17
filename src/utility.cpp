#include "utility.hpp"

sf::Vector2i to_vector2i(Direction direction)
{
    switch (direction)
    {
    case Direction::Up:
        return {0, -1};
    case Direction::Down:
        return {0, 1};
    case Direction::Left:
        return {-1, 0};
    case Direction::Right:
        return {1, 0};
    default:
        break;
    }

    return {0, 0};
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
