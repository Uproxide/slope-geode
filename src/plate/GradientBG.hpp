#pragma once

#include <cocos2d.h>

namespace plate {

//! @brief Default gradient background used for layers.
class GradientBG : public cocos2d::CCSprite {
public:
    //! @brief Create default blue gradient background.
    //! @return Gradient background.
    inline static GradientBG* create()  {
        return create({ 0x00, 0x66, 0xff });
    }

    //! @brief Create gradient background with color.
    //! @param color Background color.
    //! @return Gradient background.
    static GradientBG* create(cocos2d::ccColor3B const& color);

    //! @brief Initialize gradient background with color.
    //! @param color Background color.
    //! @return True on success, false on error.
    bool init(cocos2d::ccColor3B const& color);
};

} // namespace plate
