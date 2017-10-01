#include "rand.hpp"


std::mt19937 Random::random_engine;

int Random::uniform_int(int a, int b)
{
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(random_engine);
}

float Random::uniform_float(float a, float b)
{
    std::uniform_real_distribution<float> distribution(a, b);
    return distribution(random_engine);
}
