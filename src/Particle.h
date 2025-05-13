#pragma once
#include "config.h"
#include "util.h"

#include <SFML/Graphics.hpp>
#include <cmath>

struct Particle {
    static float constexpr G = 1500;  // Gravity
    static float constexpr TTL = 1.5; // Time To Live
    static float constexpr SCALE = 0.99;

    Particle(size_t modelIdx, sf::Color color, sf::Vector2i const& mouseClickPosition)
        : m_modelIdx(modelIdx)
        , m_ttl(TTL)
        , m_degreesPerSecond(getRandInt(50, 180))
        , m_vel(sf::Vector2f(getRandInt(-500, 500), getRandInt(100, 500)))
        , m_pos(mouseClickPosition)
        , m_deg(getRandInt(-180, 180))
        , m_color1(sf::Color(255l, 255l, 255l))
        , m_color2(color)
    {
    }

    size_t m_modelIdx;
    float m_ttl;
    float m_degreesPerSecond;
    sf::Vector2f m_vel;
    sf::Vector2f m_pos;
    float m_deg;
    sf::Color m_color1;
    sf::Color m_color2;

    void update(float dtAsSeconds);
};

inline void Particle::update(float dtAsSeconds)
{
    m_ttl -= dtAsSeconds;
    m_vel.y -= G * dtAsSeconds;
    m_pos -= m_vel * dtAsSeconds;
    m_deg += m_degreesPerSecond * dtAsSeconds;
}
