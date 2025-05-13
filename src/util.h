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

void inline loadFont(sf::Font& font, sf::Text& text)
{
    if (!font.loadFromFile("font/Cascadia.ttf")) {
        throw std::runtime_error("unable to open font file");
    }

    text.setFont(font);
    text.setCharacterSize(24); // Pixels
    text.setFillColor(sf::Color(255, 255, 255));
}
