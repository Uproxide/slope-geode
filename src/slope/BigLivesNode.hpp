#pragma once

#include <Geode/Enums.hpp>
#include <cocos2d.h>

#include "PlayerNode.hpp"

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
    PlayerNode* buildPlayer();

    int m_numLives;
    PlayerNode* m_playerNode;
};

} // namespace slope