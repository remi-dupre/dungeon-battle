#pragma once

#include <array>

#include <SFML/Graphics.hpp>

#include "../config.hpp"
#include "menu.hpp"


class MainMenu : public Menu
{
public:

    explicit MainMenu() = default;

    virtual void update() override final;
    virtual void handleInput(const sf::Event& event, const Configuration& config) override final;
    virtual MenuEvent menuEvent() override final;

    virtual bool displayGame() override final;
    virtual void render(sf::RenderTarget& target) override final;

private:

    enum Items
    {
        NewGame   = 0,
        LoadGame  = 1,
        Options   = 2,
        Quit      = 3,

        Count
    } selected_item = Items::NewGame;

    std::array<sf::Text, Items::Count> item_texts;
};
