#include "main.hpp"
#include "new_game.hpp"
#include "../ressources.hpp"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


MainMenu::MainMenu()
{
    item_texts[Items::NewGame].setString("New Game");
    item_texts[Items::LoadGame].setString("Continue");
    item_texts[Items::Options].setString("Options");
    item_texts[Items::Quit].setString("Quit");

    for (unsigned int i = 0; i < Items::Count; ++i)
        item_texts[i].setFont(RessourceManager::getFont());
}

void MainMenu::update() {}

void MainMenu::handleInput(const sf::Event& event, const Configuration& config)
{
    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == config.down_key || event.key.code == config.attack_down_key)
        selected_item = static_cast<Items>((selected_item + 1) % Items::Count);
    if (event.key.code == config.up_key || event.key.code == config.attack_up_key)
        selected_item = static_cast<Items>((selected_item + Items::Count - 1) % Items::Count);

    if (event.key.code == config.select_key || event.key.code == config.interaction_key)
        select_key_pressed = true;
}

MenuEvent MainMenu::menuEvent()
{
    MenuEvent event {};

    if (!select_key_pressed)
        return event;
    select_key_pressed = false;

    if (selected_item == Items::NewGame)
    {
        event.type = MenuEvent::NextMenu;
        event.next_menu = std::make_shared<NewGameMenu>();
    }
    else if (selected_item == Items::LoadGame)
    {
        event.type = MenuEvent::LoadGame;
        event.save_path = "save01";
        //next_menu = std::make_shared<LoadGameMenu>();
    }
    else if (selected_item == Items::Options)
    {
        //event.type = MenuEvent::NextMenu;
        //next_menu = std::make_shared<LoadGameMenu>();
    }
    else if (selected_item == Items::Quit)
    {
        event.type = MenuEvent::Quit;
    }

    return event;
}

bool MainMenu::displayGame()
{
    return false;
}

void MainMenu::render(sf::RenderTarget& target)
{
    item_texts[selected_item].setCharacterSize(30);
    item_texts[selected_item].setColor(sf::Color::Yellow);

    for (unsigned int i = 0; i < Items::Count; ++i)
    {
        if (i != selected_item)
        {
            item_texts[i].setCharacterSize(20);
            item_texts[i].setColor(sf::Color::White);
        }

        float position = static_cast<float>((14 - Items::Count) / 2 + i);
        position += (Items::Count % 2 == 0) ? 1.f : 0.f;
        position /= 15.f;

        item_texts[i].setPosition({
            static_cast<float>(Configuration::default_configuration.width) / 2.f,
            position * static_cast<float>(Configuration::default_configuration.height)
        });
        item_texts[i].setPosition(item_texts[i].getPosition() -
            vec::size(item_texts[i].getLocalBounds()) / 2.f);

        target.draw(item_texts[i]);
    }
}
