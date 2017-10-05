/**
* \file AI.hpp
* \brief Decide of the action of the monster. 
*/

#pragma once

#include <memory>


#include "control.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "utility.hpp"


struct Action;


bool has_hero(const std::vector<std::shared_ptr<Entity>>& entities);

sf::Vector2u get_hero_position(const std::vector<std::shared_ptr<Entity>>& entities);

/**
 * \brief Decide of the action of the monster.
 * \param monster The monster entity that is acting.
 * \param entities The list of entities on the map.
 * \param map The map.
 */

Action get_input_monster(const Character& monster, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map);



Action just_moving(const Character& monster, const std::vector<std::shared_ptr<Entity>>& entities, const Map& map);
