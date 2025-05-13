#pragma once

#include "Particle.h"
#include <SFML/Graphics.hpp>

#include <iostream>

inline int getRandOddInt(int const min, int const max)
{
    int const randInt = getRandInt(min, max);
    return randInt % 2 ? randInt : randInt - 1;
}

struct Engine {
    Engine();
    void run();

    sf::RenderWindow m_window;

    size_t const m_modelCount;
    std::vector<sf::VertexArray> m_models;
    std::vector<Particle> m_particles;
    float m_particleAccumulator;

    void populateModels();
    void input(float dtAsSeconds);
    void update(float dtAsSeconds);
    void draw();
};

inline Engine::Engine()
    : m_modelCount(MODEL_VARIATIONS)
    , m_particleAccumulator(0.f)
{
    m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    if (!m_window.isOpen()) {
        throw std::runtime_error("Failed to create SFML window");
    }

    auto desktop = sf::VideoMode::getDesktopMode();
    m_window.setPosition({ static_cast<int>(desktop.width / 2 - WINDOW_WIDTH / 2),
        static_cast<int>(desktop.height / 2 - WINDOW_HEIGHT / 2) });

    m_window.setFramerateLimit(TARGET_FPS);
}

inline void Engine::input(float dtAsSeconds)
{
    sf::Event event;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        m_window.close();
    }

    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
    }

    bool const mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (mouseLeftPressed) {
        sf::Vector2i const mousePos = sf::Mouse::getPosition(m_window);
        m_particleAccumulator += PARTICLES_PER_SECOND * dtAsSeconds;

        while (m_particleAccumulator >= 1.f) {
            m_particles.emplace_back(Particle(getRandInt(0, m_modelCount - 1), mousePos));
            m_particleAccumulator -= 1.f;
        }
    } else {
        m_particleAccumulator = 0.f; // reset if mouse not held
    }
}

inline void Engine::update(float dtAsSeconds)
{
    std::vector<Particle>::iterator it = m_particles.begin();

    while (it != m_particles.end()) {
        if (it->m_ttl > 0.0) {
            it->update(dtAsSeconds);
            ++it;
        } else {
            it = m_particles.erase(it);
        }
    }
}

inline void Engine::draw()
{
    m_window.clear();
    for (auto const& particle : m_particles) {
        sf::RenderStates states;
        states.transform.translate(particle.m_pos.x, particle.m_pos.y);
        states.transform.rotate(particle.m_deg);
        m_window.draw(m_models.at(particle.m_modelIdx), states);
    }
    m_window.display();
}

inline void Engine::populateModels()
{

    for (size_t i = 0; i < m_modelCount; i++) {
        int const numPoints = getRandOddInt(10, 33);
        double const dTheta = 2 * M_PI / (numPoints - 1);
        double theta = getRandInt(-180, 180) * (M_PI / 180.f);
        sf::VertexArray shape(sf::TriangleFan, numPoints + 1);

        double baseRadius = getRandInt(10, 20); // Some base size
        double outerRadius = baseRadius;
        double innerRadius = baseRadius * .6f; // Or some fixed thickness

        shape[0] = sf::Vector2f(0.f, 0.f);
        for (int j = 1; j <= numPoints; j++) {
            double r = (j % 2) ? innerRadius : outerRadius;
            double dx = r * std::cos(theta);
            double dy = r * std::sin(theta);

            shape[j] = sf::Vector2f(0 + dx, 0 + dy);

            theta += dTheta;
        }

        // shape[numPoints] = shape[1];

        m_models.emplace_back(shape);
    }
}

inline void Engine::run()
{
    sf::Clock frameClock;

    populateModels();

    // ENGINE
    while (m_window.isOpen()) {
        float const dtAsSeconds = frameClock.restart().asSeconds();

        input(dtAsSeconds);
        update(dtAsSeconds);
        draw();

        m_window.display();
    }
}
