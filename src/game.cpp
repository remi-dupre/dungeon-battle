#include "game.hpp"


Game::Game()
{
}

void Game::init()
{
    std::string config_filename = "config.ini";
    config.read(config_filename);

    sf::Uint32 style = sf::Style::Close;
    if (config.fullscreen)
        style |= sf::Style::Fullscreen;

    window.create({config.width, config.height}, "Dungeon Battle", style);

    window.setVerticalSyncEnabled(config.vsync);
    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

    if (!config.vsync) // Don't activate vertical synchronization and framerate limit at the same time
        window.setFramerateLimit(config.maxfps);
 }

void Game::run()
{
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

        display();
    }
}

void Game::update()
{
}

void Game::display()
{
    window.clear();
    renderer.display(window);
    window.draw(rect);
    window.display();
}
