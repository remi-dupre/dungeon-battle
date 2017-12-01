#include <string>

#include "spells.hpp"

Spell::Spell() :
    name("Splash"),
    damage([](unsigned int strength) -> unsigned int {return strength;}),
    targets({sf::Vector2i(1, 0), sf::Vector2i(-1, 0),
             sf::Vector2i(0, 1), sf::Vector2i(0, -1)})
{}

std::string Spell::getName()
{
    return name;
}

unsigned int Spell::getDamage(unsigned int strength)
{
    return damage(strength);
}

std::vector<sf::Vector2i> Spell::getTargets()
{
    return targets;
}
