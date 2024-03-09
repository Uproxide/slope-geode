#include "PlayerNode.hpp"

#include <Geode/binding/SimplePlayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/loader/Log.hpp>

namespace slope {

PlayerNode* PlayerNode::create() {
    auto* ret = new PlayerNode();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool PlayerNode::init() {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    auto* gameMgr = GameManager::sharedState();

    int frame = gameMgr->getPlayerFrame();
    auto color = gameMgr->colorForIdx(gameMgr->getPlayerColor());
    auto color2 = gameMgr->colorForIdx(gameMgr->getPlayerColor2()); 
    auto glowColor = gameMgr->colorForIdx(gameMgr->getPlayerGlowColor());

    setContentSize({ 30.0f, 30.0f });

    auto* inner = SimplePlayer::create(frame);
    inner->setPosition(getContentSize() / 2.0f);

    inner->setColor(color);
    inner->setSecondColor(color2);

    if (gameMgr->getPlayerGlow()) {
        inner->enableCustomGlowColor(glowColor);
        inner->m_hasGlowOutline = true;
    } else {
        inner->disableCustomGlowColor();
    }

    inner->updateColors();
    addChild(inner);

    setAnchorPoint({ 0.5f, 0.5f });
    return true;
}

} // namespace slope