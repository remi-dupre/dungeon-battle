#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window({800, 600}, "Dungeon Battle", sf::Style::Close);

    while (window.isOpen())
    {
	sf::Event event;
	while (window.pollEvent(event))
	{
	    if (event.type == sf::Event::Closed)
		window.close();
	}

	window.clear();
	window.display();
    }
}
