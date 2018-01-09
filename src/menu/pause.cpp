#include "main.hpp"
#include "pause.hpp"
#include "../ressources.hpp"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


PauseMenu::PauseMenu()
{
    for (unsigned int i = 0; i < Items::Count; ++i)
        item_texts[i].setFont(RessourceManager::getFont());

    item_texts[Items::Resume].setString("Resume");
    item_texts[Items::Save].setString("Save game");
    item_texts[Items::SaveMain].setString("Save and exit to main menu");
    item_texts[Items::Main].setString("Exit to main menu");
    item_texts[Items::SaveQuit].setString("Save and quit game");
    item_texts[Items::Quit].setString("Quit game");
}

void PauseMenu::update() {}

void PauseMenu::handleInput(const sf::Event& event, const Configuration& config)
{
    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == config.menu_key)
    {
        selected_item = Items::Resume;
        select_key_pressed = true;
    }

    if (event.key.code == config.down_key || event.key.code == config.attack_down_key)
        selected_item = static_cast<Items>((selected_item + 1) % Items::Count);
    if (event.key.code == config.up_key || event.key.code == config.attack_up_key)
        selected_item = static_cast<Items>((selected_item + Items::Count - 1) % Items::Count);

    if (event.key.code == config.select_key || event.key.code == config.interaction_key)
        select_key_pressed = true;
}

MenuEvent PauseMenu::menuEvent()
{
    MenuEvent event {};

    if (!select_key_pressed)
        return event;
    select_key_pressed = false;

    if (selected_item == Items::Resume)
    {
        event.type = MenuEvent::Resume;
    }
    else if (selected_item == Items::Save)
    {
        event.type = MenuEvent::SaveGame;
    }
    else if (selected_item == Items::SaveMain)
    {
        event.type = MenuEvent::SaveGame;

        select_key_pressed = true;
        selected_item = Items::Main;
    }
    else if (selected_item == Items::Main)
    {
        event.type = MenuEvent::NextMenu;
        event.next_menu = std::make_shared<MainMenu>();
    }
    else if (selected_item == Items::SaveQuit)
    {
        event.type = MenuEvent::SaveGame;

        select_key_pressed = true;
        selected_item = Items::Quit;
    }
    else if (selected_item == Items::Quit)
    {
        event.type = MenuEvent::Quit;
    }

    return event;
}

bool PauseMenu::displayGame()
{
    return true;
}

void PauseMenu::render(sf::RenderTarget& target)
{
    sf::RectangleShape gray_bg {{
        static_cast<float>(Configuration::default_configuration.width),
        static_cast<float>(Configuration::default_configuration.height)
    }};
    gray_bg.setPosition({0.f, 0.f});
    gray_bg.setFillColor({50, 50, 50, 150});
    target.draw(gray_bg);

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
