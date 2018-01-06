#include "pause.hpp"
#include "../ressources.hpp"


PauseMenu::PauseMenu()
{
    item_texts[Items::Resume].setFont(RessourceManager::getFont());
    item_texts[Items::SaveQuit].setFont(RessourceManager::getFont());
    item_texts[Items::Quit].setFont(RessourceManager::getFont());

    item_texts[Items::Resume].setString("Resume");
    item_texts[Items::SaveQuit].setString("Save and Quit");
    item_texts[Items::Quit].setString("Quit");
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
        event.type = MenuEvent::Resume;

    if (selected_item == Items::SaveQuit)
        event.type = MenuEvent::SaveGame;

    if (selected_item == Items::Quit)
        event.type = MenuEvent::Quit;

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

    item_texts[Items::Resume].setCharacterSize(20);
    item_texts[Items::SaveQuit].setCharacterSize(20);
    item_texts[Items::Quit].setCharacterSize(20);

    item_texts[Items::Resume].setColor(sf::Color::White);
    item_texts[Items::SaveQuit].setColor(sf::Color::White);
    item_texts[Items::Quit].setColor(sf::Color::White);

    item_texts[selected_item].setCharacterSize(30);
    item_texts[selected_item].setColor(sf::Color::Yellow);

    item_texts[Items::Resume].setPosition({
        static_cast<float>(Configuration::default_configuration.width) / 2.f,
        2.f * static_cast<float>(Configuration::default_configuration.height) / 5.f
    });
    item_texts[Items::Resume].setPosition(item_texts[Items::Resume].getPosition() -
        vec::size(item_texts[Items::Resume].getLocalBounds()) / 2.f);

    item_texts[Items::SaveQuit].setPosition({
        static_cast<float>(Configuration::default_configuration.width) / 2.f,
        static_cast<float>(Configuration::default_configuration.height) / 2.f
    });
    item_texts[Items::SaveQuit].setPosition(item_texts[Items::SaveQuit].getPosition() -
        vec::size(item_texts[Items::SaveQuit].getLocalBounds()) / 2.f);

    item_texts[Items::Quit].setPosition({
        static_cast<float>(Configuration::default_configuration.width) / 2.f,
        3.f * static_cast<float>(Configuration::default_configuration.height) / 5.f
    });
    item_texts[Items::Quit].setPosition(item_texts[Items::Quit].getPosition() -
        vec::size(item_texts[Items::Quit].getLocalBounds()) / 2.f);

    target.draw(item_texts[Items::Resume]);
    target.draw(item_texts[Items::SaveQuit]);
    target.draw(item_texts[Items::Quit]);
}
