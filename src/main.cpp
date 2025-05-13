#include <SFML/Graphics.hpp>
#include <iostream>

std::string const WINDOW_TITLE = "Particle Demo";
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;
constexpr int TARGET_FPS = 60;

struct Particle {
    Particle()
        : pos(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f)
        , shape({ 100.f, 100.f })
    {
    }

    sf::Vector2f pos;
    sf::RectangleShape shape;

    void update();
    void draw(sf::RenderWindow& m_window);
};

void initWindow(sf::RenderWindow& m_window)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    m_window.create(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Default, settings);

    if (!m_window.isOpen()) {
        throw std::runtime_error("Failed to create SFML window");
    }

    auto desktop = sf::VideoMode::getDesktopMode();
    m_window.setPosition({ static_cast<int>(desktop.width / 2 - WINDOW_WIDTH / 2),
        static_cast<int>(desktop.height / 2 - WINDOW_HEIGHT / 2) });

    m_window.setFramerateLimit(TARGET_FPS);

    std::cout << "Anti-Aliasing: " << (m_window.getSettings().antialiasingLevel ? "ON" : "OFF")
              << "\n";
}

void input(sf::RenderWindow& m_window)
{
    sf::Event event;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        m_window.close();
    }

    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) {
            std::cout << "clicked" << std::endl;
        }
    }

    // sf::Vector2i const mousePos = sf::Mouse::getPosition(m_window);
    bool const mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (mouseLeftPressed) {
        // held
    }
}

void Particle::update()
{
    pos.x++;
    pos.y++;
    shape.setPosition(pos);
}

void Particle::draw(sf::RenderWindow& m_window)
{
    m_window.draw(shape);
    m_window.display();
}

int main()
{
    // sf::Clock frameClock;

    // TESTS
    sf::RenderWindow m_window;
    initWindow(m_window);

    Particle rect;

    // ENGINE
    while (m_window.isOpen()) {
        m_window.clear();
        // float const dtAsSeconds = frameClock.restart().asSeconds();
        input(m_window);
        rect.update();
        rect.draw(m_window);
    }
}
