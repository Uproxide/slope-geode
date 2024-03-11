#include "LevelInfoNode.hpp"

#include <Geode/binding/GameLevelManager.hpp>
#include <fmt/format.h>

#include "../plate/DifficultySprite.hpp"

namespace slope {

LevelInfoNode::~LevelInfoNode() {
    CC_SAFE_RELEASE(m_level);
}

LevelInfoNode* LevelInfoNode::create(Style style, GJGameLevel* level) {
    auto* ret = new LevelInfoNode();

    if (ret && ret->init(style, level)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool LevelInfoNode::init(Style style, GJGameLevel* level) {
    char const* frame = getFrame(style);
    cocos2d::CCRect rect = { 0.0f, 0.0f, 80.0f, 80.0f };

    if (!initWithFile(frame, rect)) {
        return false;
    }

    m_level = level;
    CC_SAFE_RETAIN(m_level);

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    setContentSize({
        winSize.width - 130.0f,
        84.0f
    });

    buildName();
    buildAuthor();
    buildID();

    auto* difficultySprite = plate::DifficultySprite::create(m_level, true);
    difficultySprite->setPosition({ 40.0f, 47.0f });
    addChild(difficultySprite);

    return true;
}

char const* LevelInfoNode::getFrame(Style style) {
    switch (style) {
        case Style::Opaque: return "GJ_square02.png";
        case Style::Transparent: return "square02_001.png";
    }
}

void LevelInfoNode::buildName() {
    auto* label = cocos2d::CCLabelBMFont::create(
        m_level->m_levelName.c_str(),
        "bigFont.fnt"  
    );
    label->limitLabelWidth(getContentSize().width * 0.6f, 1.0f, 0.4f);

    // Feng shui or some shit
    label->setPosition({
        getContentSize().width / 2.0f,
        57.0f,
    });

    addChild(label);
}

void LevelInfoNode::buildAuthor() {
    // This is mildly annoying
    auto* label = cocos2d::CCLabelBMFont::create(
        fmt::format("By {}", m_level->m_creatorName.c_str()).c_str(),
        "goldFont.fnt"
    );

    label->setScale(0.8f);

    // Make name green if created by an unregistered user.
    if (m_level->m_accountID == 0) {
        label->setColor({ 0x5a, 0xff, 0xff });
    }

    label->setPosition({
        getContentSize().width / 2.0f,
        29.0f,
    });

    addChild(label);
}

void LevelInfoNode::buildID() {
    auto* label = cocos2d::CCLabelBMFont::create(
        fmt::format("#{}", m_level->m_levelID.value()).c_str(),
        "chatFont.fnt"
    );

    label->setAnchorPoint({ 1.0f, 0.0f });
    label->setScale(0.6f);
    label->setColor({ 0x25, 0x30, 0x65 });

    label->setPosition({
        getContentSize().width - 7.0f,
        7.0f,
    });

    addChild(label);
}

} // namespace slope
