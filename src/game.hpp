#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "render.hpp"


class Game
{
public:

    Game();

    void init();

    void run();

private:

    sf::RenderWindow window;
    Configuration config;
    Renderer renderer;
};
