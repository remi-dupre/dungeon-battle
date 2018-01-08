#include "../ressources.hpp"
#include "../utility.hpp"
#include "game_over.hpp"
#include "main.hpp"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


GameOverMenu::GameOverMenu(int hero_level_) :
    hero_level(hero_level_)
{
    texts.resize(2);

    auto& game_over = texts[0];
    auto& back = texts[1];

    for (auto& text : texts)
        text.setFont(RessourceManager::getFont());

    game_over.setString("Game Over !");
    game_over.setColor(sf::Color::Yellow);
    game_over.setCharacterSize(50);
    sf::Vector2f position = {
        Configuration::default_configuration.width / 2.f,
        Configuration::default_configuration.height / 3.f
    };
    game_over.setPosition(position - vec::size(game_over.getLocalBounds()) / 2.f);

    back.setString("Press <Enter> or <Esc> to exit to main menu");
    back.setCharacterSize(20);
    position = {
        Configuration::default_configuration.width / 2.f,
        2.f * Configuration::default_configuration.height / 3.f
    };
    back.setPosition(position - vec::size(back.getLocalBounds()) / 2.f);
}

void GameOverMenu::update()
{
}

void GameOverMenu::handleInput(const sf::Event& event, const Configuration& config)
{
    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == config.select_key ||
        event.key.code == config.interaction_key ||
        event.key.code == config.menu_key)
        select_key_pressed = true;
}

MenuEvent GameOverMenu::menuEvent()
{
    MenuEvent event {};
    if (select_key_pressed)
    {
        event.type = MenuEvent::NextMenu;
        event.next_menu = std::make_shared<MainMenu>();
    }

    return event;
}

bool GameOverMenu::displayGame()
{
    return true;
}

void GameOverMenu::render(sf::RenderTarget& target)
{
    sf::RectangleShape gray_bg {{
        static_cast<float>(Configuration::default_configuration.width),
        static_cast<float>(Configuration::default_configuration.height)
    }};
    gray_bg.setPosition({0.f, 0.f});
    gray_bg.setFillColor({50, 50, 50, 150});
    target.draw(gray_bg);

    for (auto& text : texts)
        target.draw(text);
}
