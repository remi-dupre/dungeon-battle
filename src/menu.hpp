/**
 * \file menu.hpp
 * \brief Contains a class representing a menu.
 */

#pragma once

#include <SFML/Graphics.hpp>

#include "config.hpp"


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

    virtual bool update() = 0;
    virtual bool handle_key(sf::Keyboard::Key key, const Configuration& config) = 0;
    virtual std::shared_ptr<Menu> next_menu() = 0;
    virtual void render(sf::RenderTarget& target) = 0;

    virtual bool display_game() = 0;

private:

};

class MainMenu : public Menu
{
public:

    explicit MainMenu() = default;

    virtual bool update() override final;
    virtual bool handle_key(sf::Keyboard::Key key, const Configuration& config) override final;
    virtual std::shared_ptr<Menu> next_menu() override final;
    virtual void render(sf::RenderTarget& target) override final;

    virtual bool display_game() override final
    {
        return false;
    };

private:

};

class PauseMenu : public Menu
{
public:

    explicit PauseMenu() = default;

    virtual bool update() override final;
    virtual bool handle_key(sf::Keyboard::Key key, const Configuration& config) override final;
    virtual std::shared_ptr<Menu> next_menu() override final;
    virtual void render(sf::RenderTarget& target) override final;

    virtual bool display_game() override final
    {
        return true;
    };

    int item = 0;

private:

    enum class Next
    {
        Nothing,
        Resume,
        Quit
    };
    Next next = Next::Nothing;
};
