#include "menu.hpp"

#include "control.hpp"


bool MainMenu::update()
{
    return false;
}

void MainMenu::handle_key(sf::Keyboard::Key key, const Configuration& config)
{

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

void PauseMenu::handle_key(sf::Keyboard::Key key, const Configuration& config)
{
    if (key == config.menu_key)
        next = Next::Resume;
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
