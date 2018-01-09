#include <string>

#include "spells.hpp"

Spell::Spell() :
    name("Splash"),
    damage([](unsigned int strength) -> unsigned int {return strength;}),
    targets({sf::Vector2i(1, 0), sf::Vector2i(-1, 0),
             sf::Vector2i(0, 1), sf::Vector2i(0, -1)})
{}

const std::string& Spell::getName() const
{
    return name;
}

unsigned int Spell::getDamage(unsigned int strength)
{
    return damage(strength);
}

const std::vector<sf::Vector2i>& Spell::getTargets() const
{
    return targets;
}
