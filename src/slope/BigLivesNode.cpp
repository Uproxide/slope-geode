#include "BigLivesNode.hpp"

#include <Geode/loader/Mod.hpp>
#include <string>

namespace slope {

BigLivesNode* BigLivesNode::create(int numLives) {
    auto* ret = new BigLivesNode();

    if (ret && ret->init(numLives)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool BigLivesNode::init(int numLives) {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    m_numLives = numLives;

    buildCross();
    buildShadow();
    buildLabel();

    m_playerNode = buildPlayer();

    return true;
}

void BigLivesNode::buildCross() {
    auto* sprite = cocos2d::CCSprite::create("cross.png"_spr);

    sprite->setScale(2.0f);
    addChild(sprite);
}

void BigLivesNode::buildShadow() {
    auto* sprite = cocos2d::CCSprite::createWithSpriteFrameName(
        "d_circle_02_001.png"
    );

    sprite->setOpacity(25);
    sprite->setPosition({ -60.0f, -36.0f });
    sprite->setScaleX(2.0f);
    sprite->setScaleY(0.5f);

    addChild(sprite);
}

void BigLivesNode::buildLabel() {
    auto str = std::to_string(m_numLives);
    auto* label = cocos2d::CCLabelBMFont::create(str.c_str(), "bigFont.fnt");

    label->setAlignment(cocos2d::kCCTextAlignmentLeft);
    label->setAnchorPoint({ 0.0f, 0.5f });
    label->setPosition({ 20.0f, 0.0f });
    label->setScale(2.0f);

    addChild(label);
}

PlayerNode* BigLivesNode::buildPlayer() {
    auto* player = PlayerNode::create();

    player->setScale(2.5f);
    player->setPosition({ -60.0f, 0.0f });

    addChild(player);
    return player;
}

} // namespace slope