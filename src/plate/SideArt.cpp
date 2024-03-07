#include "SideArt.hpp"

#include "Toolbox.hpp"

namespace plate {

SideArt* SideArt::create(char const* image) {
    auto* ret = new SideArt();

    if (ret && ret->init(image)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool SideArt::init(char const* image) {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    auto* director = cocos2d::CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    setContentSize(winSize);
    ignoreAnchorPointForPosition(true);
    setCascadeColorEnabled(true);

    auto* dl = Toolbox::createSprite(image);
    dl->setAnchorPoint({ 0.0f, 0.0f });
    dl->setPosition({ 0.0f, 0.0f });
    addChild(dl);

    auto* dr = Toolbox::createSprite(image);
    dr->setAnchorPoint({ 1.0f, 0.0f });
    dr->setFlipX(true);
    dr->setPosition({ winSize.width, 0.0f });
    addChild(dr);

    auto* ul = Toolbox::createSprite(image);
    ul->setAnchorPoint({ 0.0f, 1.0f });
    ul->setFlipY(true);
    ul->setPosition({ 0.0f, winSize.height });
    addChild(ul);

    auto* ur = Toolbox::createSprite(image);
    ur->setAnchorPoint({ 1.0f, 1.0f });
    ur->setFlipX(true);
    ur->setFlipY(true);
    ur->setPosition(winSize);
    addChild(ur);

    return true;
}

} // namespace plate
