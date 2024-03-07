#include "LivesNode.hpp"

#include <Geode/loader/Mod.hpp>
#include <string>

#include "PlayerNode.hpp"
#include "Manager.hpp"

namespace slope {

LivesNode* LivesNode::create() {
    auto* ret = new LivesNode();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool LivesNode::init() {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    // Above UILayer
    setZOrder(102);

    int lives = Manager::sharedManager()->getNumLives();

    auto* player = PlayerNode::create();
    player->setScale(0.8f);
    player->setPosition({ -22.0f, 0.0f });
    addChild(player);

    auto* cross = cocos2d::CCSprite::create("cross.png"_spr);
    addChild(cross);

    m_label = cocos2d::CCLabelBMFont::create(
        std::to_string(lives).c_str(),
        "bigFont.fnt"
    );
    m_label->setScale(0.7f);
    m_label->setAlignment(cocos2d::kCCTextAlignmentLeft);
    m_label->setAnchorPoint({ 0.0f, 0.5f });
    m_label->setPosition({ 9.0f, 0.0f });
    addChild(m_label);

    return true;
}

void LivesNode::updateLabel() {
    int lives = Manager::sharedManager()->getNumLives();
    m_label->setCString(std::to_string(lives).c_str());
}

} // namespace slope
