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

    float top = director->getScreenTop();
    float bottom = director->getScreenBottom();
    float left = director->getScreenLeft();
    float right = director->getScreenRight();

    setContentSize(winSize);
    ignoreAnchorPointForPosition(true);
    setCascadeColorEnabled(true);

    auto* dl = Toolbox::createSprite(image);
    dl->setAnchorPoint({ 0.0f, 0.0f });
    dl->setPosition({ -1.0f, -1.0f });
    addChild(dl);

    auto* dr = Toolbox::createSprite(image);
    dr->setAnchorPoint({ 1.0f, 0.0f });
    dr->setFlipX(true);
    dr->setPosition({ winSize.width + 1.0f, -1.0f });
    addChild(dr);

    auto* ul = Toolbox::createSprite(image);
    ul->setAnchorPoint({ 0.0f, 1.0f });
    ul->setFlipY(true);
    ul->setPosition({ -1.0f, winSize.height + 1.0f });
    addChild(ul);

    auto* ur = Toolbox::createSprite(image);
    ur->setAnchorPoint({ 1.0f, 1.0f });
    ur->setFlipX(true);
    ur->setFlipY(true);
    ur->setPosition({ winSize.width + 1.0f, winSize.height + 1.0f });
    addChild(ur);

    return true;
}

} // namespace plate
