#pragma once

#include <cocos2d.h>

namespace plate {

//! @brief Decompiled loading circle with fixed position and unused features
//!        removed.
class LoadingCircle : public cocos2d::CCSprite {
public:
    //! @brief Create loading circle.
    //! @return Loading circle.
    static LoadingCircle* create();

    //! @brief Initialize loading circle.
    //! @return True on success, false on failure.
    bool init() override;

    //! @brief Show and start rotate action.
    void show();

    //! @brief Fade out and remove from parent.
    void fadeAndRemove();

private:
    enum Action {
        FadeIn = 0,
        Rotate = 1,
    };
};

} // namespace plate
