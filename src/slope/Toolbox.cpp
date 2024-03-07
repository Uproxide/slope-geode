#include "Toolbox.hpp"

namespace slope {

void Toolbox::transitionFade(cocos2d::CCScene* scene) {
    auto* transition = cocos2d::CCTransitionFade::create(0.5f, scene);
    cocos2d::CCDirector::sharedDirector()->replaceScene(transition);
}

} // namespace slope
