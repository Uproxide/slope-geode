#pragma once

#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <cocos2d.h>

#include "LevelSearcher.hpp"
#include "../plate/LoadingCircle.hpp"

namespace slope {

//! @brief Basic "world" layer.
//! @todo Rewrite this entire class
class WorldLayer : public cocos2d::CCLayer,
                   public LevelSearcherDelegate,
                   public FLAlertLayerProtocol {
public:
    ~WorldLayer();

    static cocos2d::CCScene* scene();
    static WorldLayer* create();
    bool init() override;

    void keyBackClicked() override;
    void onEnterTransitionDidFinish() override;

    void searchFinished(GJGameLevel* level) override;
    void searchFailed() override;

    void FLAlert_Clicked(FLAlertLayer* alert, bool second) override;

private:
    enum Alert {
        Error,
        Quit,
    };

    void onBack(cocos2d::CCObject* sender);
    void onPlay(cocos2d::CCObject* sender);

    void onQuit(bool yes);

    plate::LoadingCircle* m_loadingCircle;
    cocos2d::CCNode* m_playButton;
    GJGameLevel* m_level;
};

} // namespace slope