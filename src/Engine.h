#pragma once

#include "../lib/Color_Space.h"
#include "Particle.h"
#include "config.h"
#include "util.h"

#include <SFML/Graphics.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

struct Engine {
    Engine();
    void run();

    sf::RenderWindow m_window;
    float m_frameRate;

    sf::Vector2i m_prevMousePos;

    size_t const m_modelCount;
    std::vector<sf::VertexArray> m_models;
    std::vector<Particle> m_particles;

    sf::Shader m_tintShader;
    size_t m_currColorIdx;
    std::vector<sf::Vector3f> m_colors;

    sf::Font m_font;
    sf::Text m_text;

    void populateModels();
    void input(float dtAsSeconds);
    void update(float dtAsSeconds);
    void draw();

    void populateParticles(sf::Vector2i const& mousePos, float const dtAsSeconds);
};

inline Engine::Engine()
    : m_frameRate(0)
    , m_prevMousePos(sf::Vector2i(0, 0))
    , m_modelCount(MODEL_VARIATIONS)
    , m_currColorIdx(0l)
    , m_colors(clrspc::get_rainbow_colors(PARTICLES_PER_SECOND * SECONDS_PER_RAINBOW_CYCLE))
{
    m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    m_window.setFramerateLimit(TARGET_FPS);

    if (!m_window.isOpen()) {
        throw std::runtime_error("Failed to create SFML window");
    }

    auto desktop = sf::VideoMode::getDesktopMode();
    m_window.setPosition({ static_cast<int>(desktop.width / 2 - WINDOW_WIDTH / 2),
        static_cast<int>(desktop.height / 2 - WINDOW_HEIGHT / 2) });

    if (!m_tintShader.loadFromFile("shader/color_tint.frag", sf::Shader::Fragment)) {
        throw std::runtime_error("Failed to load color tint shader");
    }

    loadFont(m_font, m_text);
}

inline void Engine::populateParticles(sf::Vector2i const& mousePos, float const dtAsSeconds)
{
    size_t const particleCount
        = static_cast<size_t>(std::round(PARTICLES_PER_SECOND * dtAsSeconds));

    for (size_t i = 0; i < particleCount; ++i) {
        sf::Vector2i particlePos = mousePos;

        if (m_prevMousePos != sf::Vector2i(0, 0)) {
            float t = static_cast<float>(i) / particleCount;
            sf::Vector2f interpolatedPos
                = (1 - t) * sf::Vector2f(m_prevMousePos) + t * sf::Vector2f(mousePos);
            particlePos = static_cast<sf::Vector2i>(interpolatedPos);
        }

        m_particles.emplace_back(
            getRandInt(0, m_modelCount - 1), m_colors[m_currColorIdx], particlePos);

        m_currColorIdx = (m_currColorIdx + 1) % m_colors.size();
    }
}

inline void Engine::input(float dtAsSeconds)
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            m_window.close();
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i const mousePos = sf::Mouse::getPosition(m_window);
        populateParticles(mousePos, dtAsSeconds);
        m_prevMousePos = mousePos;
    } else {
        m_prevMousePos = sf::Vector2i(0, 0);
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

    std::stringstream ostream;
    ostream << std::setw(11) << "Particles: " << m_particles.size() << '\n'
            << std::setw(11) << "FPS: " << std::fixed << std::setprecision(4)
            << m_frameRate - dtAsSeconds;
    m_text.setString(ostream.str());
}

inline void Engine::draw()
{
    m_window.clear();
    for (auto const& particle : m_particles) {
        sf::RenderStates states;
        states.transform.translate(particle.m_pos.x, particle.m_pos.y);
        states.transform.rotate(particle.m_deg);

        sf::Glsl::Vec4 tintColor(
            particle.m_colorBase.x, particle.m_colorBase.y, particle.m_colorBase.z, 1);

        sf::Glsl::Vec4 centerColor(
            particle.m_colorCenter.x, particle.m_colorCenter.y, particle.m_colorCenter.z, 1);

        // set base color
        m_tintShader.setUniform("tintColor", tintColor);
        m_tintShader.setUniform("centerColor", centerColor);

        // Set the radius and edge for blending
        m_tintShader.setUniform("radius", 0.f); // start at center
        m_tintShader.setUniform("edge", 10.f);  // smoothly fade for 10 units

        states.shader = &m_tintShader;
        m_window.draw(m_models.at(particle.m_modelIdx), states);
    }

    m_window.draw(m_text);
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

        // Center vertex
        shape[0].position = sf::Vector2f(0.f, 0.f);
        shape[0].texCoords = sf::Vector2f(0.f, 0.f); // Origin

        for (int j = 1; j <= numPoints; j++) {
            double r = (j % 2) ? innerRadius : outerRadius;
            double dx = r * std::cos(theta);
            double dy = r * std::sin(theta);

            shape[j].position = sf::Vector2f(dx, dy);

            // Use texCoords as local (model) position
            shape[j].texCoords = sf::Vector2f(dx, dy);

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

    while (m_window.isOpen()) {
        float const dtAsSeconds = frameClock.restart().asSeconds();

        if (dtAsSeconds > 0.f) {
            m_frameRate = 1.f / dtAsSeconds;
        }

        input(dtAsSeconds);
        update(dtAsSeconds);
        draw();

        m_window.display();
    }
}
