#include "PlayerNode.hpp"

#include <Geode/binding/SimplePlayer.hpp>

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

    setContentSize({ 30.0f, 30.0f });

    auto* inner = SimplePlayer::create(2);
    inner->setPosition(getContentSize() / 2.0f);
    addChild(inner);

    setAnchorPoint({ 0.5f, 0.5f });
    return true;
}

} // namespace slope