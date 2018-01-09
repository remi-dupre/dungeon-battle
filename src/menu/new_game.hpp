#pragma once

#include <array>

#include <SFML/Graphics.hpp>

#include "../config.hpp"
#include "menu.hpp"


class NewGameMenu : public Menu
{
public:

    explicit NewGameMenu();

    virtual void update() override final;
    virtual void handleInput(const sf::Event& event, const Configuration& config) override final;
    virtual MenuEvent menuEvent() override final;

    virtual bool displayGame() override final;
    virtual void render(sf::RenderTarget& target) override final;

private:

    enum Characters
    {
        Warrior    = 0,
        Rogue      = 1,
        Wizard     = 2,
        Angel      = 3,

        CharactersCount
    } selected_character = Characters::Warrior;

    std::array<sf::Sprite, CharactersCount> character_sprites;
    std::array<sf::Text, CharactersCount> character_texts;

    enum Items
    {
        Back       = 0,
        SelectChar = 1,
        Play       = 2,

        Count
    } selected_item = Items::SelectChar;

    std::array<sf::Text, Items::Count> item_texts;
};
