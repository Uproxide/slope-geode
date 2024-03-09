#pragma once

#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/LevelDownloadDelegate.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <cocos2d.h>

#include "AssetDownloader.hpp"
#include "LevelSearcher.hpp"
#include "ProgressCircle.hpp"

#include "../plate/LoadingCircle.hpp"

namespace slope {

//! @brief Introduces a level to the player.
class IntroLayer : public cocos2d::CCLayer,
                   public LevelDownloadDelegate,
                   public AssetDownloaderDelegate,
                   public FLAlertLayerProtocol,
                   public LevelSearcherDelegate {
public:
    IntroLayer() :
        m_assetDownloader(nullptr),
        m_levelReady(false),
        m_transitionReady(false),
        m_cancelAlertOpen(false) {}
    
    ~IntroLayer() override;

    static cocos2d::CCScene* scene(GJGameLevel* level);
    static IntroLayer* create(GJGameLevel* level);
    bool init(GJGameLevel* level);

    void onEnterTransitionDidFinish() override;

    void levelDownloadFinished(GJGameLevel* level) override;
    void levelDownloadFailed(int error) override;

    void assetDownloadFinished() override;
    void assetDownloadFailed() override;

    void keyBackClicked() override;
    void FLAlert_Clicked(FLAlertLayer* alert, bool second) override;

    void searchFinished(GJGameLevel* level) override;
    void searchFailed() override;

private:
    void buildBackground();
    void buildBackButton();
    void buildLevelInfo();
    void buildLives();
    plate::LoadingCircle* buildLoadingCircle();
    ProgressCircle* buildProgressCircle();

    void downloadLevel(); //!< Begin downloading level.
    void downloadAssets(); //!< Begin downloading level assets.

    //! @brief Check if we can play the level.
    //! @details Runs in a loop until m_canPlay is set.
    void checkReady();

    void play(); //!< Begin level play sequence.
    void playStep2(); //!< Preload music.
    void playStep3(); //!< Create PlayLayer.
    void loadLevelStep(); //!< Create level objects.
    void playStep4(); //!< Switch to PlayLayer scene.

    enum Alert {
        Skip = 1,
        Cancel,
        Error,
    };

    void onSkip(bool yes); //!< Skip alert handler.
    void onCancel(bool yes); //!< Cancel alert handler.
    void onError(); //!< Error alert handler.

    void onBack(cocos2d::CCObject* sender); //!< Back button handler.

    GJGameLevel* m_level;
    AssetDownloader* m_assetDownloader;

    plate::LoadingCircle* m_loadingCircle;
    ProgressCircle* m_progressCircle;
    cocos2d::CCScene* m_playScene;

    bool m_transitionReady;
    bool m_levelReady;
    bool m_cancelAlertOpen;
};

} // namespace slope
