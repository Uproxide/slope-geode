#pragma once

#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <cocos2d.h>

namespace plate {

//! @brief Wrapper around GJDifficultySprite with a less silly API.
class DifficultySprite : public cocos2d::CCNode {
public:
    //! @brief Create difficulty sprite.
    //! @param level Level to create from.
    //! @param extra Create stars/moons label.
    //! @return Difficulty sprite.
    static DifficultySprite* create(GJGameLevel* level, bool extra);
    
    //! @brief Initialize difficulty sprite.
    //! @param level Level to create from.
    //! @param extra Create stars/moons label.
    //! @return True on success, false on failure.
    bool init(GJGameLevel* level, bool extra);
};

} // namespace plate
