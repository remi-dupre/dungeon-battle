#include "menu.hpp"

#include "control.hpp"


bool MainMenu::update()
{
    return false;
}

bool MainMenu::handle_key(sf::Keyboard::Key key, const Configuration& config)
{
    return false;
}

std::shared_ptr<Menu> MainMenu::next_menu()
{
    return nullptr;
}

void MainMenu::render(sf::RenderTarget& target)
{

}


bool PauseMenu::update()
{
    if (next != Next::Nothing)
        return true;
    return false;
}

bool PauseMenu::handle_key(sf::Keyboard::Key key, const Configuration& config)
{
    if (key == config.menu_key)
        next = Next::Resume;

    if (key == config.down_key || key == config.attack_down_key)
        item = (item + 1) % 2;
    if (key == config.up_key || key == config.attack_up_key)
        item = (item + 1) % 2;

    if (key == config.select_key || key == config.interaction_key)
    {
        if (item == 0)
            next = Next::Resume;
        if (item == 1)
        {
            next = Next::Quit;
            return true;
        }
    }

    return false;
}

std::shared_ptr<Menu> PauseMenu::next_menu()
{
    if (next == Next::Resume)
        return nullptr;
    return nullptr;
}

void PauseMenu::render(sf::RenderTarget& target)
{

}
