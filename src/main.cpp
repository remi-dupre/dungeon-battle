#include <SFML/Graphics.hpp>

#include "config.hpp"


int main(int argc, char *argv[])
{
    Configuration config;
    std::string config_filename = "config.ini";

    if (argc > 1)
        config_filename = argv[1];

    config.read(config_filename);

    sf::Uint32 style = sf::Style::Close;
    if (config.fullscreen)
        style |= sf::Style::Fullscreen;

    sf::RenderWindow window({config.width, config.height}, "Dungeon Battle", style);
    window.setVerticalSyncEnabled(config.vsync);
    if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
        window.setFramerateLimit(config.maxfps);

    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

    sf::RectangleShape rect({40.f, 60.f});
    rect.setOrigin(10.f, 10.f);
    rect.setFillColor(sf::Color::Red);

    sf::Clock timer;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float time_since_last_frame = timer.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            rect.setPosition(rect.getPosition() + time_since_last_frame * sf::Vector2f(0.f, -100.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            rect.setPosition(rect.getPosition() + time_since_last_frame * sf::Vector2f(0.f, 100.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            rect.setPosition(rect.getPosition() + time_since_last_frame * sf::Vector2f(-100.f, 0.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            rect.setPosition(rect.getPosition() + time_since_last_frame * sf::Vector2f(100.f, 0.f));
        }

        window.clear();
        window.draw(rect);
        window.display();
    }
}
