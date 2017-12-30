#include "../src/ressources.hpp"


class RessourcesTester : public CxxTest::TestSuite
{
    friend RessourceManager;

public:
    void testAnimationsLoading()
    {
        RessourceManager::setRessourcesPath("tests/data/");
        RessourceManager::loadAnimations();

        auto animation = RessourceManager::getAnimation(EntitySprite::Slime);

        TS_ASSERT_EQUALS(animation.sprite_rect.left, 1);
        TS_ASSERT_EQUALS(animation.sprite_rect.top, 2);
        TS_ASSERT_EQUALS(animation.sprite_rect.width, 32);
        TS_ASSERT_EQUALS(animation.sprite_rect.height, 33);

        TS_ASSERT_EQUALS(animation.getFrame(Direction::Down, 0.f).left, 1);
        TS_ASSERT_EQUALS(animation.getFrame(Direction::Down, 0.f).top, 1);

        TS_ASSERT_EQUALS(animation.getFrame(Direction::Down, 0.3f).left, 33);
        TS_ASSERT_EQUALS(animation.getFrame(Direction::Down, 0.3f).top, 1);

        TS_ASSERT_EQUALS(animation.getFrame(Direction::Down, 0.6f).left, 65);
        TS_ASSERT_EQUALS(animation.getFrame(Direction::Down, 0.6f).top, 1);

        TS_ASSERT_EQUALS(animation.getFrame(Direction::Down, 0.8f).left, 97);
        TS_ASSERT_EQUALS(animation.getFrame(Direction::Down, 0.8f).top, 1);

        TS_ASSERT_EQUALS(animation.animation[Direction::Down].size(), 4);
        TS_ASSERT_EQUALS(animation.animation[Direction::Left].size(), 1);

    }

};
