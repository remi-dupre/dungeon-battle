#include <SFML/Graphics.hpp>

#include "game.hpp"


int main(int argc, char *argv[])
{
    Game game;

    game.init();

    game.run();

    return 0;
}
