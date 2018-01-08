#include "../ressources.hpp"
#include "main.hpp"
#include "new_game.hpp"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


NewGameMenu::NewGameMenu()
{
    character_sprites[Characters::Warrior].setTexture(RessourceManager::getTexture(Textures::Warrior));
    character_sprites[Characters::Rogue  ].setTexture(RessourceManager::getTexture(Textures::Rogue ));
    character_sprites[Characters::Wizard ].setTexture(RessourceManager::getTexture(Textures::Wizard));
    character_sprites[Characters::Angel  ].setTexture(RessourceManager::getTexture(Textures::Angel));

    character_sprites[Characters::Warrior].setTextureRect(
        RessourceManager::getAnimation(EntitySprite::Warrior).getFrame(Direction::Down, 0.f));
    character_sprites[Characters::Rogue ].setTextureRect(
        RessourceManager::getAnimation(EntitySprite::Rogue ).getFrame(Direction::Down, 0.f));
    character_sprites[Characters::Wizard].setTextureRect(
        RessourceManager::getAnimation(EntitySprite::Wizard).getFrame(Direction::Down, 0.f));
    character_sprites[Characters::Angel].setTextureRect(
        RessourceManager::getAnimation(EntitySprite::Angel).getFrame(Direction::Down, 0.f));

    float scale = 3.f;

    character_sprites[Characters::Warrior].setScale({scale, scale});
    character_sprites[Characters::Rogue ].setScale({scale, scale});
    character_sprites[Characters::Wizard].setScale({scale, scale});
    character_sprites[Characters::Angel].setScale({scale, scale});

    sf::Vector2f position = {
        1.f * static_cast<float>(Configuration::default_configuration.width) / 8.f,
        static_cast<float>(Configuration::default_configuration.height ) / 2.f
    };
    sf::Vector2f offset = static_cast<sf::Vector2f>
        (vec::size(RessourceManager::getSpriteRect(EntitySprite::Warrior))) / 2.f;
    character_sprites[Characters::Warrior].setPosition(position - scale * offset);

    position = {
        3.f * static_cast<float>(Configuration::default_configuration.width) / 8.f,
        static_cast<float>(Configuration::default_configuration.height) / 2.f
    };
    offset = static_cast<sf::Vector2f>
        (vec::size(RessourceManager::getSpriteRect(EntitySprite::Rogue ))) / 2.f;
    character_sprites[Characters::Rogue].setPosition(position - scale * offset);

    position = {
        5.f * static_cast<float>(Configuration::default_configuration.width) / 8.f,
        static_cast<float>(Configuration::default_configuration.height ) / 2.f
    };
    offset = static_cast<sf::Vector2f>
        (vec::size(RessourceManager::getSpriteRect(EntitySprite::Wizard))) / 2.f;
    character_sprites[Characters::Wizard].setPosition(position - scale * offset);

    position = {
        7.f * static_cast<float>(Configuration::default_configuration.width) / 8.f,
        static_cast<float>(Configuration::default_configuration.height ) / 2.f
    };
    offset = static_cast<sf::Vector2f>
        (vec::size(RessourceManager::getSpriteRect(EntitySprite::Angel))) / 2.f;
    character_sprites[Characters::Angel].setPosition(position - scale * offset);


    std::string names[CharactersCount];
    names[Characters::Warrior] = "Warrior"s;
    names[Characters::Rogue ] = "Rogue"s;
    names[Characters::Wizard] = "Wizard"s;
    names[Characters::Angel] = "Angel"s;
    for (unsigned int i = 0; i < CharactersCount; ++i)
    {
        character_texts[i].setFont(RessourceManager::getFont());
        character_texts[i].setString(names[i]);
    }

    item_texts[Items::Back].setString("Back");
    item_texts[Items::SelectChar].setString("Select a character");
    item_texts[Items::Play].setString("Start new game");
    for (unsigned int i = 0; i < Items::Count; ++i)
        item_texts[i].setFont(RessourceManager::getFont());
}

void  NewGameMenu::update() {}

void NewGameMenu::handleInput(const sf::Event& event, const Configuration& config)
{
    if (event.type != sf::Event::KeyPressed)
        return;

    if (selected_item == Items::SelectChar)
    {
        if (event.key.code == config.right_key || event.key.code == config.attack_right_key)
            selected_character = static_cast<Characters>((selected_character + 1) % CharactersCount);
        else if (event.key.code == config.left_key || event.key.code == config.attack_left_key)
            selected_character = static_cast<Characters>
                ((selected_character + CharactersCount - 1) % CharactersCount);
    }

    if (event.key.code == config.up_key || event.key.code == config.attack_up_key)
        selected_item = static_cast<Items>((selected_item + Items::Count - 1) % Items::Count);
    else if (event.key.code == config.down_key || event.key.code == config.attack_down_key)
        selected_item = static_cast<Items>((selected_item + 1) % Items::Count);

    if (event.key.code == config.select_key || event.key.code == config.interaction_key)
        select_key_pressed = true;
}

MenuEvent NewGameMenu::menuEvent()
{
    MenuEvent event {};

    if (!select_key_pressed)
        return event;
    select_key_pressed = false;

    if (selected_item == Items::Back)
    {
        event.type = MenuEvent::NextMenu;
        event.next_menu = std::make_shared<MainMenu>();
    }
    else if (selected_item == Items::Play || selected_item == Items::SelectChar)
    {
        event.type = MenuEvent::NewGame;
        event.save_path = "save01";

        switch (selected_character)
        {
        case Characters::Warrior:
            event.hero_class = Class::Warrior;
            break;
        case Characters::Rogue:
            event.hero_class = Class::Rogue;
            break;
        case Characters::Wizard:
            event.hero_class = Class::Wizard;
            break;
        case Characters::Angel:
            event.hero_class = Class::Angel;
            break;
        default:
            break;
        }
    }

    return event;
}

bool NewGameMenu::displayGame()
{
    return false;
}

void NewGameMenu::render(sf::RenderTarget& target)
{
    target.draw(character_sprites[Characters::Warrior]);
    target.draw(character_sprites[Characters::Rogue  ]);
    target.draw(character_sprites[Characters::Wizard ]);
    target.draw(character_sprites[Characters::Angel  ]);

    character_texts[selected_character].setCharacterSize(30);
    character_texts[selected_character].setColor({255, 0, 255});
    if (selected_item != Items::SelectChar)
    {
        character_texts[selected_character].setCharacterSize(25);
        character_texts[selected_character].setColor({150, 0, 150});
    }
    for (unsigned int i = 0; i < CharactersCount; ++i)
    {
        if (i != selected_character)
        {
            character_texts[i].setCharacterSize(20);
            character_texts[i].setColor(sf::Color::White);
        }

        sf::Vector2f position = {
            (1.f + i * 2.f) * static_cast<float>(Configuration::default_configuration.width) / 8.f,
            static_cast<float>(Configuration::default_configuration.height) / 2.f + 100.f
        };
        character_texts[i].setPosition(position - vec::size(character_texts[i].getLocalBounds()) / 2.f);
        target.draw(character_texts[i]);
    }

    item_texts[selected_item].setCharacterSize(30);
    item_texts[selected_item].setColor(sf::Color::Yellow);
    for (unsigned int i = 0; i < Items::Count; ++i)
    {
        if (i != selected_item)
        {
            item_texts[i].setCharacterSize(20);
            item_texts[i].setColor(sf::Color::White);
        }

        sf::Vector2f position {Configuration::default_configuration.width / 2.f, 0.f};

        if (i == Items::Back)
            position.y = 80.f;
        if (i == Items::SelectChar)
            position.y = 160.f;
        if (i == Items::Play)
            position.y = 480.f;

        item_texts[i].setPosition(position - vec::size(item_texts[i].getLocalBounds()) / 2.f);
        target.draw(item_texts[i]);
    }
}
