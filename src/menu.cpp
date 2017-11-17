#include "menu.hpp"


Menu::Menu(const Configuration& config) :
    current_line(0),
    chosen_character(0)
{
    sf::Uint32 style = sf::Style::Close | sf::Style::Resize;
    if (config.fullscreen)
        style |= sf::Style::Fullscreen;

    window.create({config.width, config.height}, "Dungeon Battle", style);
    window.setVerticalSyncEnabled(config.vsync);
    window.setSize({config.scalefactor * config.width, config.scalefactor * config.height});

}

void Menu::run()
{
    while (true)
    {

    }
}
