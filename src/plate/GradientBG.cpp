#include "GradientBG.hpp"

namespace plate {

GradientBG* GradientBG::create(cocos2d::ccColor3B const& color) {
    auto* ret = new GradientBG();

    if (ret && ret->init(color)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool GradientBG::init(cocos2d::ccColor3B const& color) {
    if (!initWithFile("GJ_gradientBG.png")) {
        return false;
    }

    auto* director = cocos2d::CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto size = getContentSize();

    setAnchorPoint({ 0.0f, 0.0f });
    setScaleX((winSize.width + 10.0f) / size.width);
    setScaleY((winSize.height + 10.0f) / size.height);
    setPosition({ -5.0f, -5.0f });
    setColor(color);

    return true;
}

} // namespace plate
