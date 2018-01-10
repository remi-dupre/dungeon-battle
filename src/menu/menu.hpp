/**
 * \file menu.hpp
 * \brief Contains a class representing a menu.
 */

#pragma once

#include <SFML/Graphics.hpp>

#include "../config.hpp"
#include "../entity.hpp"


class Menu;

struct MenuEvent
{
    enum
    {
        Nothing,
        Quit,
        Resume,
        NewGame,
        LoadGame,
        SaveGame,
        NextMenu
    } type = Nothing;
    std::string save_path = Configuration::user_path;
    Class hero_class = Class::Warrior;
    std::shared_ptr<Menu> next_menu = nullptr;
};

/**
 * \brief Represent a menu.
*/
class Menu
{
public:

    /**
     * \brief Menu default constructor.
     */
    explicit Menu() = default;

    virtual void update() = 0;
    virtual void handleInput(const sf::Event& event, const Configuration& config) = 0;
    virtual MenuEvent menuEvent() = 0;

    virtual bool displayGame() = 0;
    virtual void render(sf::RenderTarget& target) = 0;

protected:
    bool select_key_pressed = false;
};
