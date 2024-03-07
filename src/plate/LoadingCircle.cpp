#include "LoadingCircle.hpp"

namespace plate {

LoadingCircle* LoadingCircle::create() {
    auto* ret = new LoadingCircle();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
};

bool LoadingCircle::init() {
    if (!initWithFile("loadingCircle.png")) {
        return false;
    }

    setBlendFunc({ GL_SRC_ALPHA, GL_ONE }); // Additive blending
    setOpacity(0);
    setZOrder(105);

    return true;
}

void LoadingCircle::show() {
    auto* fadeAction = cocos2d::CCFadeTo::create(0.4f, 200);
    fadeAction->setTag(Action::FadeIn);

    auto* rotateAction = cocos2d::CCRepeatForever::create(
        cocos2d::CCRotateBy::create(1.0f, 360.0f)
    );
    rotateAction->setTag(Action::Rotate);

    runAction(rotateAction);
    runAction(fadeAction);
}

void LoadingCircle::fadeAndRemove() {
    stopActionByTag(Action::FadeIn);

    auto* action = cocos2d::CCSequence::create(
        cocos2d::CCFadeTo::create(0.3f, 0),
        cocos2d::CCCallFunc::create(
            this,
            callfunc_selector(LoadingCircle::removeMeAndCleanup)
        ),
        nullptr
    );

    runAction(action);
}

} // namespace plate
