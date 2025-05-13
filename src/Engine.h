#pragma once

#include "Particle.h"
#include <SFML/Graphics.hpp>

#include <iostream>

struct Engine {

    Engine();
    void run();

    sf::RenderWindow m_window;

    size_t const m_modelCount;
    std::vector<sf::RectangleShape> m_models;
    std::vector<Particle> m_particles;

    void input();
    void update(float dtAsSeconds);
    void draw();
};

inline Engine::Engine()
    : m_modelCount(10)
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

inline void Engine::input()
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

    // sf::Vector2i const mousePos = sf::Mouse::getPosition(m_window);
    bool const mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (mouseLeftPressed) {
        sf::Vector2i const mousePos = sf::Mouse::getPosition(m_window);
        m_particles.emplace_back(mousePos, getRandInt(0, m_modelCount - 1));
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

inline void Engine::run()
{
    sf::Clock frameClock;

    for (size_t i = 0; i < m_modelCount; i++) {
        sf::RectangleShape shape
            = sf::RectangleShape(sf::Vector2f(getRandInt(10, 20), getRandInt(10, 20)));
        shape.setOrigin(shape.getSize() / 2.f);
        m_models.emplace_back(shape);
    }

    // ENGINE
    while (m_window.isOpen()) {
        float const dtAsSeconds = frameClock.restart().asSeconds();

        input();
        update(dtAsSeconds);
        draw();

        m_window.display();
    }
}
