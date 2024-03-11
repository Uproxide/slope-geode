#pragma once

#include <Geode/Enums.hpp>
#include <cocos2d.h>

#include "../plate/SimplePlayer.hpp"

namespace slope {

//! @brief Big lives display for IntroLayer.
class BigLivesNode : public cocos2d::CCNode {
public:
    static BigLivesNode* create(int numLives);
    bool init(int numLives);

private:
    void buildCross();
    void buildShadow();
    void buildLabel();
    plate::SimplePlayer* buildPlayer();

    int m_numLives;
    plate::SimplePlayer* m_playerNode;
};

} // namespace slope
