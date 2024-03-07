#pragma once

#include <cocos2d.h>

namespace slope {

//! @todo Rewrite this entire class.
class PresetSelectLayer : public cocos2d::CCLayer {
public:
    static cocos2d::CCScene* scene();
    static PresetSelectLayer* create();
    bool init() override;

    void keyBackClicked() override;

private:
    void onBack(cocos2d::CCObject* sender);
    void onSelect(cocos2d::CCObject* sender);
    
    void buildPresets();
};

} // namespace slope
