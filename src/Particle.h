#pragma once
#include "config.h"
#include "util.h"

#include <SFML/Graphics.hpp>
#include <cmath>

struct Particle {
    static float constexpr G = 1500; // Gravity
    static float constexpr TTL = 1;  // Time To Live
    static float constexpr SCALE = 0.99;

    Particle(size_t modelIdx, sf::Vector3f color, sf::Vector2i const& mouseClickPosition)
        : m_modelIdx(modelIdx)
        , m_ttl(TTL)
        , m_degreesPerSecond(getRandInt(50, 180))
        , m_vel(sf::Vector2f(getRandInt(-500, 500), getRandInt(100, 500)))
        , m_pos(mouseClickPosition)
        , m_deg(getRandInt(-180, 180))
        , m_colorBase(color)
        , m_colorCenter(color + sf::Vector3f(.2f, .2f, .2f))
    {
    }

    size_t m_modelIdx;
    float m_ttl;
    float m_degreesPerSecond;
    sf::Vector2f m_vel;
    sf::Vector2f m_pos;
    float m_deg;
    sf::Vector3f m_colorBase;
    sf::Vector3f m_colorCenter;

    void update(float dtAsSeconds);
};

inline void Particle::update(float dtAsSeconds)
{
    auto decayToBlack = [](sf::Vector3f& color, double rate = .025) {
        color.x = (color.x > rate) ? color.x - rate : 0;
        color.y = (color.y > rate) ? color.y - rate : 0;
        color.z = (color.z > rate) ? color.z - rate : 0;
    };

    decayToBlack(m_colorBase);
    decayToBlack(m_colorCenter);

    m_ttl -= dtAsSeconds;
    m_vel.y -= G * dtAsSeconds;
    m_pos -= m_vel * dtAsSeconds;
    m_deg += m_degreesPerSecond * dtAsSeconds;
}
