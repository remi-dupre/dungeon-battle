#include <SFML/Graphics.hpp>

/**
 * \brief Main function
 */
int main(int argc, char *argv[])
{
    sf::RenderWindow window({800, 600}, "Dungeon Battle", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

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
