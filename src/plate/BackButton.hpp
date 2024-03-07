#pragma once

#include <cocos2d.h>

namespace plate {

//! @brief Back button used on most layers.
class BackButton : public cocos2d::CCMenu {
public:
    //! @brief Create default pink back button.
    //! @param target Target object for callback.
    //! @param callback Callback function.
    //! @return Back button.
    inline static BackButton* create(
        cocos2d::CCObject* target,
        cocos2d::SEL_MenuHandler callback
    ) {
        return create("GJ_arrow_03_001.png", target, callback);
    }

    //! @brief Create back button with image.
    //! @param image Image or frame path.
    //! @param target Target object for callback.
    //! @param callback Callback function.
    //! @return Back button.
    static BackButton* create(
        char const* image,
        cocos2d::CCObject* target,
        cocos2d::SEL_MenuHandler callback
    );

    //! @brief Initialize back button.
    //! @param image Image or frame path.
    //! @param target Target object for callback.
    //! @param callback Callback function.
    //! @return True on success, false on failure.
    bool init(
        char const* image,
        cocos2d::CCObject* target,
        cocos2d::SEL_MenuHandler callback
    );

private:
    //! @brief Add controller hint.
    void addControllerHint();
};

} // namespace plate
