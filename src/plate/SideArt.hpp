#pragma once

#include <cocos2d.h>

namespace plate {

//! @brief Side art the game puts on gradient layers.
//! @todo Write documentation for this.
class SideArt : public cocos2d::CCNodeRGBA {
public:
    static SideArt* create(char const* image);
    bool init(char const* image);
};

} // namespace plate
