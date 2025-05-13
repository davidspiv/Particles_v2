#pragma once

#include <random>

inline int getRandInt(int const min, int const max)
{
    return (std::rand() % (max - min + 1)) + min;
}

inline int getRandOddInt(int const min, int const max)
{
    int const randInt = getRandInt(min, max);
    return randInt % 2 ? randInt : randInt - 1;
}
