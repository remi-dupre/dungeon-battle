#include "rand.hpp"


std::mt19937 Random::random_engine;

template <typename S>
void Random::seed(S seed)
{
    random_engine.seed(seed);
}

int Random::uniform_int(int a, int b)
{
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(random_engine);
}
