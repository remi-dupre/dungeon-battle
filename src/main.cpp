#include <cstring>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "config.hpp"


int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    Configuration config;
    std::string config_filename = "config.ini";

    config.read(config_filename);

    sf::RenderWindow window({config.width, config.height}, "Dungeon Battle", sf::Style::Close);
    window.setVerticalSyncEnabled(config.vsync);

    sf::RectangleShape rect({40.f, 60.f});
    rect.setOrigin(10.f, 10.f);
    rect.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            rect.setPosition(rect.getPosition() + sf::Vector2f(0.f, -10.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            rect.setPosition(rect.getPosition() + sf::Vector2f(0.f, 10.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            rect.setPosition(rect.getPosition() + sf::Vector2f(-10.f, 0.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            rect.setPosition(rect.getPosition() + sf::Vector2f(10.f, 0.f));
        }

        window.clear();
        window.draw(rect);
        window.display();
    }
}
