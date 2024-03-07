#pragma once

#include <Geode/binding/GJGameLevel.hpp>
#include <cocos2d.h>
#include <cocos-ext.h>

namespace slope {

//! @brief Level info display for IntroLayer and PauseLayer.
class LevelInfoNode : public cocos2d::extension::CCScale9Sprite {
public:
    enum class Style {
        Opaque,
        Transparent,
    };

    ~LevelInfoNode() override;

    static LevelInfoNode* create(Style style, GJGameLevel* level);
    bool init(Style style, GJGameLevel* level);

private:
    char const* getFrame(Style style);

    void buildName();
    void buildAuthor();
    void buildID();

    GJGameLevel* m_level;
};

} // namespace slope
