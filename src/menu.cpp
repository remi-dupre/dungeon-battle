#include "menu.hpp"


Menu::Menu(const Configuration& config_) :
    current_line(0),
    chosen_character(0)
{
    sf::Uint32 style = sf::Style::Close | sf::Style::Resize;
    if (config_.fullscreen)
        style |= sf::Style::Fullscreen;

    window.create({config_.width, config_.height}, "Dungeon Battle", style);
    window.setVerticalSyncEnabled(config_.vsync);
    window.setSize({config_.scalefactor * config_.width, config_.scalefactor * config_.height});

}

void Menu::run()
{
    while (true)
    {

    }
}
