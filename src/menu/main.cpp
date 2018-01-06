#include "main.hpp"
#include "../ressources.hpp"


void MainMenu::handleInput(const sf::Event& event, const Configuration& config)
{
}

MenuEvent MainMenu::menuEvent()
{
    MenuEvent event {};

    if (!select_key_pressed)
        return event;
    select_key_pressed = false;

    if (selected_item == Items::Quit)
        event.type = MenuEvent::Quit;

    return event;
}

bool MainMenu::displayGame()
{
    return false;
}

void MainMenu::render(sf::RenderTarget& target)
{
}
