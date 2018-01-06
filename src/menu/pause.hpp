#include <array>

#include <SFML/Graphics.hpp>

#include "../config.hpp"
#include "menu.hpp"


class PauseMenu : public Menu
{
public:

    explicit PauseMenu();

    virtual void update() override final;
    virtual void handleInput(const sf::Event& event, const Configuration& config) override final;
    virtual MenuEvent menuEvent() override final;

    virtual bool displayGame() override final;
    virtual void render(sf::RenderTarget& target) override final;

private:

    enum Items
    {
        Resume   = 0,
        SaveQuit = 1,
        Quit     = 2,

        Count
    } selected_item = Items::Resume;

    std::array<sf::Text, Items::Count> item_texts;
};
