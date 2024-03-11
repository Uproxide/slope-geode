#include "DifficultySprite.hpp"

#include <string>
#include "Toolbox.hpp"

namespace plate {

DifficultySprite* DifficultySprite::create(GJGameLevel* level, bool extra) {
    auto* ret = new DifficultySprite();

    if (ret && ret->init(level, extra)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

// I think this is inlined
static inline int demonIconForDifficulty(int demonDifficulty) {
    // This makes sense
    switch (demonDifficulty) {
        default: return 6;
        case 3: return 7;
        case 4: return 8;
        case 5: return 9;
        case 6: return 10;
    }
}

bool DifficultySprite::init(GJGameLevel* level, bool extra) {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    int difficulty = level->getAverageDifficulty();
    
    if (level->m_demon) {
        difficulty = demonIconForDifficulty(level->m_demonDifficulty);
    }

    if (level->m_autoLevel) {
        difficulty = -1;
    }

    auto* impl = GJDifficultySprite::create(
        difficulty,
        static_cast<GJDifficultyName>(1)
    );
    impl->updateFeatureStateFromLevel(level);

    if (level->m_demon) {
        impl->setPositionY(5.0f);
    }

    addChild(impl);

    if (extra) {
        char const* frame = "star_small01_001.png";
        if (level->m_levelLength == 5) {
            frame = "moon_small01_001.png";
        }

        auto* currencyIcon = Toolbox::createSprite(frame);
        currencyIcon->setPosition({ 8.0f, -30.0f });
        addChild(currencyIcon);

        auto* currencyLabel = cocos2d::CCLabelBMFont::create(
            std::to_string(level->m_stars).c_str(),
            "bigFont.fnt"
        );
        currencyLabel->setAnchorPoint({ 1.0f, 0.5f });
        currencyLabel->setScale(0.4f);
        currencyLabel->setPosition({ 0.0f, -30.0f });
        addChild(currencyLabel);

        // I eat kids
        if (level->m_demon) {
            currencyIcon->setPositionY(-34.0f);
            currencyLabel->setPositionY(-34.0f);
        }
    }

    return true;
}

} // namespace plate
