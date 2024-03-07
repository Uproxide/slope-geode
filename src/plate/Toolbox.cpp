#include "Toolbox.hpp"

namespace plate {

cocos2d::CCSprite* Toolbox::createSprite(char const* image) {
    auto* sprite = cocos2d::CCSprite::create();

    if (sprite && initSprite(sprite, image)) {
        return sprite;
    } else {
        return nullptr;
    }
}

bool Toolbox::initSprite(cocos2d::CCSprite* sprite, char const* image) {
    if (sprite->initWithFile(image)) {
        return true;
    }

    if (sprite->initWithSpriteFrameName(image)) {
        return true;
    }

    return false;
}

} // namespace plate
