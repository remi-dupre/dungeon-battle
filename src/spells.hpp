/**
 * \file spells.hpp
 * \brief Representation of spells
 */

#pragma once

#include <functional>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>


/**
 * \brief Represents damaging spells
 */
class Spell
{
public:

    /**
     * \brief Create a dummy spell
     */
    Spell();

    /**
     * \brief Return the name of the spell
     */
    const std::string& getName() const;

    /**
     * \brief Return the damage done by the spell
     * \param strength The strength of the caster of the spell
     */
    unsigned int getDamage(unsigned int strength);

    /**
     * \brief Return the relative position of the targets of the spell
     */
    const std::vector<sf::Vector2i>& getTargets() const;

protected:

    std::string name; ///< The name of the spell
    std::function<unsigned int(unsigned int)> damage; ///< The damage curve of the spell
    std::vector<sf::Vector2i> targets; ///< The relative position of the targets of the spell
};
