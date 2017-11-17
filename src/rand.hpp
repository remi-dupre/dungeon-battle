/**
 * \file rand.hpp
 * Define random functions.
 */

#pragma once

#include <random>


enum class RandomType
{
    Game,
    Render,
    Generation
};

/**
 * \brief Handle the generation of random numbers
 */
template <RandomType T>
class Random
{
public:

    Random() = delete;

    /**
     * \brief Seed the internal engine
     * \param seed The seed to give to the engine
     */
    template <typename S>
    static void seed(S seed)
    {
        random_engine.seed(seed);
    }

    /**
     * \brief Return a random integer
     * \param a The minimum integer possible
     * \param b The maximum integer possible
     *
     * Returns a random integer between `a` and `b`,
     * uniformly distributed on the given interval
     */
    static int uniform_int(int a, int b);

    /**
     * \brief Return a random float number
     * \param a The minimum float possible
     * \param b The maximum float possible
     *
     * Returns a random float number between `a` and `b`,
     * uniformly distributed on the given interval
     */
    static float uniform_float(float a, float b);

private:

    static std::mt19937 random_engine; ///< The internal random engine
};

template <RandomType T>
std::mt19937 Random<T>::random_engine;

template <RandomType T>
int Random<T>::uniform_int(int a, int b)
{
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(random_engine);
}

template <RandomType T>
float Random<T>::uniform_float(float a, float b)
{
    std::uniform_real_distribution<float> distribution(a, b);
    return distribution(random_engine);
}

typedef Random<RandomType::Game> Rand;
typedef Random<RandomType::Render> RandRender;
typedef Random<RandomType::Generation> RandGen;
