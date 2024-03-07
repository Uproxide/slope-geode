#pragma once

#include <cocos2d.h>
#include <cocos-ext.h>

namespace plate {

//! @brief Static class with several helper functions.
class Toolbox {
public:
    //! @brief Create a sprite from either a file or sprite frame name.
    //! @param image Image path or sprite frame name.
    //! @return Sprite.
    static cocos2d::CCSprite* createSprite(char const* image);

    //! @brief Initialize a sprite from either a file or sprite frame name.
    //! @param sprite Target sprite.
    //! @param image Image path or sprite frame name
    //! @return True on success, false on failure.
    static bool initSprite(cocos2d::CCSprite* sprite, char const* image);
};

} // namespace plate
