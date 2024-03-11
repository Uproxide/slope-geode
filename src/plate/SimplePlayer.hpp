#pragma once

#include <cocos2d.h>

namespace plate {

//! @brief Wrapper around SimplePlayer with a less silly API.
class SimplePlayer : public cocos2d::CCNode {
public:
    static SimplePlayer* create();
    bool init() override;
};

} // namespace plate
