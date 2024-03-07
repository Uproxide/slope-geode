#include "ProgressCircle.hpp"

#include <Geode/loader/Mod.hpp>

namespace slope {

ProgressCircle* ProgressCircle::create() {
    auto* ret = new ProgressCircle();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

bool ProgressCircle::init() {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    auto* base = cocos2d::CCSprite::create("progress_01.png"_spr);
    auto* meter = cocos2d::CCSprite::create("progress_02.png"_spr);

    m_progressTimer = cocos2d::CCProgressTimer::create(meter);

    addChild(base);
    addChild(m_progressTimer);

    return true;
}

} // namespace slope
