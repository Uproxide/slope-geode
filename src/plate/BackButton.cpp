#include "BackButton.hpp"

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/PlatformToolbox.hpp>

#include "Toolbox.hpp"

namespace plate {

BackButton* BackButton::create(
    char const* image,
    cocos2d::CCObject* target,
    cocos2d::SEL_MenuHandler callback
) {
    auto* ret = new BackButton();

    if (ret && ret->init(image, target, callback)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

bool BackButton::init(
    char const* image,
    cocos2d::CCObject* target,
    cocos2d::SEL_MenuHandler callback
) {
    if (!cocos2d::CCMenu::init()) {
        return false;
    }
    
    // CCMenu has this set by default.
    // Turning it off corrects the node's position.
    ignoreAnchorPointForPosition(false);

    // Position at the top left corner of the screen.
    auto* director = cocos2d::CCDirector::sharedDirector();
    setPosition({
        director->getScreenLeft() + 24.0f,
        director->getScreenTop() - 23.0f,
    });

    // Create menu item.
    auto* sprite = Toolbox::createSprite(image);
    auto* menuItem = CCMenuItemSpriteExtra::create(sprite, target, callback);
    menuItem->setSizeMult(1.6f);

    addChild(menuItem);

    // Resize menu to fit children.
    // This makes the node look nicer in DevTools.
    setContentSize(menuItem->getContentSize());
    menuItem->setPosition(menuItem->getContentSize() / 2.0f);

    if (PlatformToolbox::isControllerConnected()) {
        addControllerHint();
    }

    return true;
}

void BackButton::addControllerHint() {
    cocos2d::CCPoint center = getContentSize() / 2.0f;

    auto* sprite = Toolbox::createSprite("controllerBtn_B_001.png");
    sprite->setPosition({
        center.x + 28.0f,
        center.y + 10.0f,
    });
    sprite->setScale(0.7f);

    addChild(sprite);
}

} // namespace plate
