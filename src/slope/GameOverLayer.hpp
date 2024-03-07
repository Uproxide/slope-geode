#pragma once

#include <cocos2d.h>

namespace slope {

class GameOverLayer : public cocos2d::CCLayer {
public:
    GameOverLayer() : m_canExit(false) {}

    static cocos2d::CCScene* scene();
    static GameOverLayer* create();
    bool init() override;

    bool ccTouchBegan(
        cocos2d::CCTouch* touch,
        cocos2d::CCEvent* event
    ) override;
    void keyDown(cocos2d::enumKeyCodes key) override;

    void onEnterTransitionDidFinish() override;

private:
    void exit();

    bool m_canExit;
};

} // namespace slope