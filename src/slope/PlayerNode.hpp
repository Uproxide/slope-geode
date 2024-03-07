#pragma once

#include <cocos2d.h>

namespace slope {

//! @brief Temporary player node.
//! @todo Move this into plate.
class PlayerNode : public cocos2d::CCNode {
public:
    static PlayerNode* create();
    bool init() override;
};

} // namespace slope
