#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <vector>

#include "entity.hpp"
#include "utility.hpp"


unsigned int currentId = 0;

Entity::Entity(EntityType type_, Interaction interaction_, sf::Vector2i position_, Direction orientation_) :
    id(++currentId),
    controller_id(0),
    type(type_),
    interaction(interaction_),
    position(position_),
    orientation(orientation_),
    moving(false),
    attacking(false),
    attacked(false)
{}

Entity::Entity(EntityType type_, Interaction interaction_, sf::Vector2i position_, Direction orientation_, unsigned int controller_id_) :
    id(++currentId),
    controller_id(controller_id_),
    type(type_),
    interaction(interaction_),
    position(position_),
    orientation(orientation_),
    moving(false),
    attacking(false),
    attacked(false)
{}

unsigned int Entity::getId() const
{
    return id;
}

unsigned int Entity::getControllerId() const
{
    return controller_id;
}

EntityType Entity::getType() const
{
    return type;
}

Interaction Entity::getInteraction() const
{
    return interaction;
}

sf::Vector2i Entity::getPosition() const
{
    return position;
}

void Entity::setPosition(sf::Vector2i position_)
{
    old_position = position;
    position = position_;
}

Direction Entity::getOrientation() const
{
    return orientation;
}

void Entity::setOrientation(Direction orientation_)
{
    if (orientation_ != Direction::None)
        orientation = orientation_;
}

sf::Vector2i Entity::getOldPosition() const
{
    return old_position;
}

void Entity::setMoving(bool moving_)
{
    moving = moving_;
}

bool Entity::isMoving() const
{
    return moving;
}

void Entity::setAttacking(bool attacking_)
{
    attacking = attacking_;
}

bool Entity::isAttacking() const
{
    return attacking;
}

void Entity::setAttacked(bool attacked_)
{
    attacked = attacked_;
}

bool Entity::isAttacked() const
{
    return attacked;
}

int Entity::zIndex()
{
    switch(type)
    {
    case EntityType::None:
        return 0;
    case EntityType::Hero:
        return 1;
    case EntityType::Monster:
        return 1;
    case EntityType::Stairs:
        return 0;
    default:
        return 0;
    }
}

Item::Item(const std::string& name_,
        const sf::Vector2i& position_,
        int level_,
        int experience_,
        int hpMax_,
        int hp_,
        int strength_,
        int defense_,
        int sightRadius_) :
    Entity(EntityType::Item, Interaction::None, position_, Direction::None),
    name(name_),
    level(level_),
    experience(experience_),
    hpMax(hpMax_),
    hp(hp_),
    strength(strength_),
    defense(defense_),
    sightRadius(sightRadius_)
{}

int Item::getLevel() const
{
    return level;
}

int Item::getExperience() const
{
    return experience;
}

int Item::getHpMax() const
{
    return hpMax;
}

int Item::getHp() const
{
    return hp;
}

int Item::getStrength() const
{
    return strength;
}

int Item::getDefense() const
{
    return defense;
}

int Item::getSightRadius() const
{
    return sightRadius;
}

Character::Character(EntityType type_,
                     Interaction interaction_,
                     sf::Vector2i position_,
                     Direction orientation_,
                     Class character_class,
                     unsigned int hpMax_,
                     unsigned int strength_) :
    Character(type_, interaction_, position_, orientation_, character_class, hpMax_, strength_, 0)
{
    if (type_ == EntityType::Monster)
    {
        experience = 5;
    }
}

Character::Character(EntityType type_,
                     Interaction interaction_,
                     sf::Vector2i position_,
                     Direction orientation_,
                     Class character_class,
                     unsigned int hpMax_,
                     unsigned int strength_,
                     unsigned int controller_id_) :
    Entity(type_, interaction_, position_, orientation_, controller_id_),
    character_class(character_class),
    level(1),
    experienceCurve([](unsigned int lvl) -> unsigned int {return 10*lvl;}),
    experience(0),
    hpMax(hpMax_),
    hp(hpMax_),
    strength(strength_),
    defense(0),
    sightRadius(0),
    inventory(std::vector<Item>()),
    inventorySize(0),
    spells(std::vector<Spell> ({Spell()}))
{
    if (type_ == EntityType::Monster)
    {
        experience = 5;
        character_class = Class::Slime;
    }
}

Class Character::getClass() const
{
    return character_class;
}

unsigned int Character::getLevel() const
{
    int c = level;

    for (const Item& item : inventory)
    {
        c += item.getLevel();
    }

    return static_cast<unsigned int>(std::max(c, 0));
}

void Character::setLevel(unsigned int level_)
{
    level = level_;
}

void Character::levelUp()
{
    if (experience > experienceCurve(level))
    {
        experience -= experienceCurve(level);
        level++;
        hpMax += 5;
        hp = hpMax;
        levelUp();
    }
}

unsigned int Character::getHpMax() const
{
    int c = hpMax;

    for (const Item& item : inventory)
    {
        c += item.getHpMax();
    }

    return static_cast<unsigned int>(std::max(c, 0));
}

void Character::setHpMax(unsigned int hpMax_)
{
    hpMax = hpMax_;
}

unsigned int Character::getHp() const
{
    int c = hp;

    for (const Item& item : inventory)
    {
        c += item.getHp();
    }

    return static_cast<unsigned int>(std::max(c, 0));
}

void Character::setHp(unsigned int hp_)
{
    hp = hp_;
}

void Character::addHp(int hp_)
{
    (static_cast<int>(hp) < -hp_) ? hp = 0 : hp = std::min(hp+hp_, hpMax);
}

bool Character::isAlive() const
{
    return (getHp() > 0);
}

unsigned int Character::getStrength() const
{
    int c = strength;

    for (const Item& item : inventory)
    {
        c += item.getStrength();
    }

    return static_cast<unsigned int>(std::max(c, 0));
}

void Character::setStrength(unsigned int strength_)
{
    strength = strength_;
}

unsigned int Character::getDefense() const
{
    int c = defense;

    for (const Item& item : inventory)
    {
        c += item.getDefense();
    }

    return static_cast<unsigned int>(std::max(c, 0));
}

void Character::setDefense(unsigned defense_)
{
    defense = defense_;
}

unsigned int Character::getSightRadius() const
{
    int c = sightRadius;

    for (const Item& item : inventory)
    {
        c += item.getSightRadius();
    }

    return static_cast<unsigned int>(std::max(c, 0));
}

void Character::setSightRadius(unsigned int sightRadius_)
{
    sightRadius = sightRadius_;
}

unsigned int Character::getExperience() const
{
    return experience;
}

void Character::setExperience(unsigned int experience_)
{
    experience = experience_;
}

void Character::addExperience(unsigned int experience_)
{
    experience += experience_;
    levelUp();
}

void Character::awardExperience(const Character& target)
{
    if (!target.isAlive())
    {
        addExperience(target.getExperience());
    }
}

bool Character::roomInInventory()
{
    return (inventorySize > inventory.size());
}

void Character::pickUp(Item item)
{
    inventory.push_back(item);
}


std::vector<Spell> Character::getSpells()
{
    return spells;
}


bool has_hero(const std::vector<std::shared_ptr<Entity>>& entities)
{
    for(const auto& entity : entities)
    {
        if(entity->getType() == EntityType::Hero)
        {
            return true;
        }
    }

    return false;
}


sf::Vector2i get_hero_position(const std::vector<std::shared_ptr<Entity>>& entities)
{
    for(const auto& entity : entities)
    {
        if(entity->getType() == EntityType::Hero)
        {
            return entity->getPosition();
        }
    }

    assert(false);
    return {0, 0};
}
