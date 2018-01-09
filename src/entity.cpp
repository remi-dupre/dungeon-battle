#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <memory>
#include <vector>

#include "entity.hpp"
#include "utility.hpp"


unsigned int currentId = 0;

Entity::Entity(EntityType type_, Interaction interaction_,
               sf::Vector2i position_, Direction orientation_,
               Controller controller_) :
    id(++currentId),
    type(type_),
    interaction(interaction_),
    controller(controller_),
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

Controller Entity::getController() const
{
    return controller;
}

EntityType Entity::getType() const
{
    return type;
}

std::shared_ptr<Entity> Entity::copy() const
{
    std::shared_ptr<const Entity> shared = shared_from_this();

    switch (type)
    {
        case EntityType::Hero: {
            auto ext = std::static_pointer_cast<const Character>(shared);
            auto copy = std::make_shared<Character>(*ext);
            return std::static_pointer_cast<Entity>(copy);
        } break;

        case EntityType::Monster: {
            auto ext = std::static_pointer_cast<const Character>(shared);
            auto copy = std::make_shared<Character>(*ext);
            return std::static_pointer_cast<Entity>(copy);
        } break;

        case EntityType::Stairs: {
            auto ext = std::static_pointer_cast<const Entity>(shared);
            return std::make_shared<Entity>(*ext);
        } break;

        case EntityType::Item: {
            auto ext = std::static_pointer_cast<const Item>(shared);
            auto copy = std::make_shared<Item>(*ext);
            return std::static_pointer_cast<Entity>(copy);
        } break;

        case EntityType::None:
            break;
    }

    // return std::make_shared<Entity>(*shared);
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

int Entity::zIndex() const
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
                     Class character_class_,
                     unsigned int hpMax_,
                     unsigned int strength_,
                     Controller controller_) :
    Entity(type_, interaction_, position_, orientation_, controller_),
    character_class(character_class_),
    level(1),
    experienceCurve([](unsigned int lvl) -> unsigned int {return 10*lvl;}),
    experience(0),
    hpMax(hpMax_),
    hp(hpMax_),
    strength(strength_),
    defense(0),
    sightRadius(0),
    inventory({}),
    inventorySize(0),
    spells(std::vector<Spell> ({Spell()}))
{}

Character::Character(Class character_class_, sf::Vector2i position_) :
    Character::Character(
        EntityType::Monster,
        Interaction::None,
        position_,
        Direction::Left,
        character_class_,
        StatManager::hp[character_class_],
        StatManager::strength[character_class_]
    )
{
    experience = StatManager::xp[character_class_];
    sightRadius = StatManager::sightradius[character_class_];
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
    (static_cast<int>(hp) < -hp_) ? hp = 0 : hp = std::min(hp + hp_, hpMax);
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

bool Character::roomInInventory() const
{
    return (inventorySize > inventory.size());
}

void Character::pickUp(Item item)
{
    inventory.push_back(item);
}


const std::vector<Spell>& Character::getSpells() const
{
    return spells;
}

Class randomClass()
{
    int r = rand() % 3;
    std::vector<int> monsters = {3,5,6};
    return static_cast<Class>(monsters[r]);
}

std::map<Class, int> StatManager::xp{};
std::map<Class, int> StatManager::strength{};
std::map<Class, int> StatManager::hp{};
std::map<Class, int> StatManager::sightradius{};

bool StatManager::loadStats()
{
    std::ifstream stats_file(Configuration::data_path + "monsters-stats");
    std::string monster_class_str;

    if(!stats_file.is_open())
        return false;

    xp.clear(); strength.clear(); hp.clear(); sightradius.clear();

    while (stats_file >> monster_class_str)
    {
        Class character_class = Class::None;
        if (monster_class_str == "Slime")
            character_class = Class::Slime;
        else if (monster_class_str == "Goat")
            character_class = Class::Goat;
        else if (monster_class_str == "Bat")
            character_class = Class::Bat;

        stats_file >> xp[character_class];
        stats_file >> strength[character_class];
        stats_file >> hp[character_class];
        stats_file >> sightradius[character_class];

    }
    return true;
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



std::ostream& operator<<(std::ostream& stream, const Entity& entity)
{
    stream.write(reinterpret_cast<const char*>(&entity.type), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.interaction), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.controller), sizeof(uint32_t));

    stream.write(reinterpret_cast<const char*>(&entity.position.x), sizeof(int32_t));
    stream.write(reinterpret_cast<const char*>(&entity.position.y), sizeof(int32_t));

    stream.write(reinterpret_cast<const char*>(&entity.old_position.x), sizeof(int32_t));
    stream.write(reinterpret_cast<const char*>(&entity.old_position.y), sizeof(int32_t));

    stream.write(reinterpret_cast<const char*>(&entity.orientation), sizeof(uint8_t));

    return stream;
}

std::istream& operator>>(std::istream& stream, Entity& entity)
{
    stream.read(reinterpret_cast<char*>(&entity.type), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.interaction), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.controller), sizeof(uint32_t));

    stream.read(reinterpret_cast<char*>(&entity.position.x), sizeof(int32_t));
    stream.read(reinterpret_cast<char*>(&entity.position.y), sizeof(int32_t));

    stream.read(reinterpret_cast<char*>(&entity.old_position.x), sizeof(int32_t));
    stream.read(reinterpret_cast<char*>(&entity.old_position.y), sizeof(int32_t));

    stream.read(reinterpret_cast<char*>(&entity.orientation), sizeof(uint8_t));

    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Character& entity)
{
    stream << (*static_cast<const Entity*>(&entity));

    stream.write(reinterpret_cast<const char*>(&entity.character_class), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.level), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.experience), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.hpMax), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.hp), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.strength), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.defense), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&entity.sightRadius), sizeof(uint32_t));

    return stream;
}

std::istream& operator>>(std::istream& stream, Character& entity)
{
    stream >> (*static_cast<Entity*>(&entity));

    stream.read(reinterpret_cast<char*>(&entity.character_class), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.level), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.experience), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.hpMax), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.hp), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.strength), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.defense), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&entity.sightRadius), sizeof(uint32_t));

    return stream;
}
