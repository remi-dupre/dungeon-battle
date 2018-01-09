#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../config.hpp"
#include "menu.hpp"


class GameOverMenu : public Menu
{
public:

    explicit GameOverMenu(int hero_level);

    virtual void update() override final;
    virtual void handleInput(const sf::Event& event, const Configuration& config) override final;
    virtual MenuEvent menuEvent() override final;

    virtual bool displayGame() override final;
    virtual void render(sf::RenderTarget& target) override final;

private:

    std::vector<sf::Text> texts;
    int hero_level;
};
