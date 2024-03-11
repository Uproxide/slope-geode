#include "IntroLayer.hpp"

#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/loader/Log.hpp>

// FMODAudioEngine breaks unless this is included manually.
#include <Geode/fmod/fmod.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

#include "Geode/binding/GJGameLevel.hpp"
#include "LevelInfoNode.hpp"
#include "BigLivesNode.hpp"
#include "PresetSelectLayer.hpp"
#include "Manager.hpp"
#include "Toolbox.hpp"

#include "../plate/GradientBG.hpp"
#include "../plate/BackButton.hpp"
#include "../plate/SideArt.hpp"

namespace slope {

IntroLayer::~IntroLayer() {
    CC_SAFE_RELEASE(m_level);
    CC_SAFE_RELEASE(m_assetDownloader);
}

cocos2d::CCScene* IntroLayer::scene(GJGameLevel* level) {
    auto* scene = cocos2d::CCScene::create();
    auto* layer = IntroLayer::create(level);

    scene->addChild(layer);
    return scene;
}

IntroLayer* IntroLayer::create(GJGameLevel* level) {
    auto* ret = new IntroLayer();

    if (ret && ret->init(level)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool IntroLayer::init(GJGameLevel* level) {
    if (!cocos2d::CCLayer::init()) {
        return false;
    }

    m_level = level;
    CC_SAFE_RETAIN(m_level);

    setKeypadEnabled(true);
    setTouchEnabled(true);

    buildBackground();

    auto* sideArt = plate::SideArt::create("gauntletCorner_001.png");
    sideArt->setColor({ 76, 76, 76 });
    addChild(sideArt);

    buildBackButton();
    buildLevelInfo();
    buildLives();

    m_loadingCircle = buildLoadingCircle();
    m_progressCircle = buildProgressCircle();

    checkReady();

    return true;
}

void IntroLayer::onEnterTransitionDidFinish() {
    cocos2d::CCLayer::onEnterTransitionDidFinish();
    m_transitionReady = true;

    downloadLevel();
}

void IntroLayer::buildBackground() {
    auto* background = plate::GradientBG::create({ 32, 32, 32 });
    background->setZOrder(-1);
    addChild(background);
}

void IntroLayer::buildBackButton() {
    auto* button = plate::BackButton::create(
        this,
        menu_selector(IntroLayer::onBack)
    );

    addChild(button);
}

void IntroLayer::buildLevelInfo() {
    auto* node = LevelInfoNode::create(
        LevelInfoNode::Style::Opaque,
        m_level
    );

    node->setPosition({
        getContentSize().width / 2.0f,
        getContentSize().height - 47.0f,
    });

    addChild(node);
}

void IntroLayer::buildLives() {
    int numLives = Manager::sharedManager()->getNumLives();
    auto* node = BigLivesNode::create(numLives);
    
    node->setPosition({
        getContentSize().width / 2.0f,
        (getContentSize().height / 2.0f) - 25.0f,
    });

    addChild(node);
}

plate::LoadingCircle* IntroLayer::buildLoadingCircle() {
    auto* node = plate::LoadingCircle::create();

    node->setScale(0.5f);
    node->setPosition({
        getContentSize().width - 30.0f,
        30.0f,
    });

    addChild(node);
    return node;
}

ProgressCircle* IntroLayer::buildProgressCircle() {
    auto* node = ProgressCircle::create();
    
    node->setScale(0.5f);
    node->setPercentage(0.0f);
    node->setPosition({
        getContentSize().width - 30.0f,
        30.0f,
    });

    node->setVisible(false);

    addChild(node);
    return node;
}

void IntroLayer::downloadLevel() {
    auto* mgr = GameLevelManager::sharedState();

    mgr->m_levelDownloadDelegate = this;
    mgr->downloadLevel(m_level->m_levelID, m_level->m_gauntletLevel);

    m_loadingCircle->show();
}

void IntroLayer::levelDownloadFinished(GJGameLevel* level) {
    CC_SAFE_RELEASE(m_level);
    m_level = level;
    CC_SAFE_RETAIN(m_level);

    downloadAssets();
}

void IntroLayer::levelDownloadFailed(int error) {
    m_loadingCircle->fadeAndRemove();

    auto* alert = FLAlertLayer::create(
        this,
        "Error",
        "Level download failed. Returning to main menu.",
        "OK",
        nullptr
    );

    alert->setTag(Alert::Error);
    alert->show();
}

void IntroLayer::downloadAssets() {
    m_assetDownloader = AssetDownloader::create(m_level);
    CC_SAFE_RETAIN(m_assetDownloader);

    m_assetDownloader->setDelegate(this);
    m_assetDownloader->download();
}

void IntroLayer::assetDownloadFinished() {
    geode::log::debug("IntroLayer::assetDownloadFinished()");

    m_loadingCircle->fadeAndRemove();
    m_levelReady = true;
}

void IntroLayer::assetDownloadFailed() {
    geode::log::debug("IntroLayer::assetDownloadFailed()");

    m_loadingCircle->fadeAndRemove();
    auto* alert = FLAlertLayer::create(
        this,
        "Warning",
        "Some assets failed to download. Would you like to skip?",
        "No",
        "Yes"
    );

    alert->setTag(Alert::Skip);
    alert->show();
}

void IntroLayer::checkReady() {
    // geode::log::debug("IntroLayer::checkPlay()");

    if (m_levelReady && m_transitionReady && !m_cancelAlertOpen) {
        play();
        return;
    }

    auto* callFunc = cocos2d::CCCallFunc::create(
        this,
        callfunc_selector(IntroLayer::checkReady)
    );

    auto* delay = cocos2d::CCDelayTime::create(0.5f);
    auto* sequence = cocos2d::CCSequence::create(delay, callFunc, nullptr);

    runAction(sequence);
}

void IntroLayer::keyBackClicked() {
    if (m_levelReady) {
        return;
    }

    onBack(nullptr);
}

void IntroLayer::FLAlert_Clicked(FLAlertLayer* alert, bool second) {
    geode::log::debug("IntroLayer::FLAlert_Clicked()");

    switch (alert->getTag()) {
    case Alert::Skip:
        onSkip(second);
        break;

    case Alert::Cancel:
        onCancel(second);
        break;

    case Alert::Error:
        onError();
        break;
    }
}

void IntroLayer::onError() {
    geode::log::debug("IntroLayer::onError()");

    Manager::sharedManager()->endGame();

    auto* scene = PresetSelectLayer::scene();
    Toolbox::transitionFade(scene);
}

void IntroLayer::onSkip(bool yes) {
    if (!yes) {
        m_levelReady = true;
        return;
    }

    auto* searcher = Manager::sharedManager()->getLevelSearcher();
    searcher->setDelegate(this);
    searcher->search();
}

void IntroLayer::onCancel(bool yes) {
    if (!yes) {
        m_cancelAlertOpen = false;
        return;
    }

    Manager::sharedManager()->endGame();
    auto* scene = PresetSelectLayer::scene();
    Toolbox::transitionFade(scene);
}

void IntroLayer::onBack(cocos2d::CCObject* sender) {
    auto* alert = FLAlertLayer::create(
        this,
        "Warning",
        "Are you sure you want to cancel?",
        "No",
        "Yes"
    );

    alert->setTag(Alert::Cancel);
    alert->show();

    m_cancelAlertOpen = true;
}

void IntroLayer::play() {
    geode::log::debug("IntroLayer::play()");

    m_progressCircle->setVisible(true);

    auto* mgr = GameManager::sharedState();
    mgr->m_unkBool11 = true; // ???

    auto* callFunc = cocos2d::CCCallFunc::create(
        this,
        callfunc_selector(IntroLayer::playStep2)
    );

    auto* delay = cocos2d::CCDelayTime::create(0.0f);
    auto* sequence = cocos2d::CCSequence::create(delay, callFunc, nullptr);
    runAction(sequence);
}

void IntroLayer::playStep2() {
    geode::log::debug("IntroLayer::playStep2()");

    auto* engine = FMODAudioEngine::sharedEngine();
    engine->loadMusic(
        m_level->getAudioFileName(),
        1.0f,
        0.0f,
        1.0f,
        true,
        0,
        0
    );

    auto* callFunc = cocos2d::CCCallFunc::create(
        this,
        callfunc_selector(IntroLayer::playStep3)
    );

    auto* delay = cocos2d::CCDelayTime::create(0.0f);
    auto* sequence = cocos2d::CCSequence::create(delay, callFunc, nullptr);
    runAction(sequence);
}

void IntroLayer::playStep3() {
    geode::log::debug("IntroLayer::playStep3()");

    auto* mgr = GameManager::sharedState();
    mgr->m_sceneEnum = 3; // ???

    m_playScene = PlayLayer::scene(m_level, false, true);
    CC_SAFE_RETAIN(m_playScene);

    auto* callFunc = cocos2d::CCCallFunc::create(
        this,
        callfunc_selector(IntroLayer::loadLevelStep)
    );

    auto* delay = cocos2d::CCDelayTime::create(0.0f);
    auto* sequence = cocos2d::CCSequence::create(delay, callFunc, nullptr);
    runAction(sequence);
}

void IntroLayer::loadLevelStep() {
    auto* playLayer = static_cast<PlayLayer*>(
        m_playScene->getChildren()->objectAtIndex(0)
    );

    // I still don't fully understand how this works.
    playLayer->processCreateObjectsFromSetup();

    //! @todo I was too lazy to update bindings for m_unk2a80 and I'm not
    //!       entirely sure why. Fix this.
    m_progressCircle->setPercentage(playLayer->m_unk2a80 * 100.0f);

    cocos2d::CCCallFunc* callFunc;
    if (playLayer->m_unk2a80 < 1.0f) {
        callFunc = cocos2d::CCCallFunc::create(
            this,
            callfunc_selector(IntroLayer::loadLevelStep)
        );
    } else {
        callFunc = cocos2d::CCCallFunc::create(
            this,
            callfunc_selector(IntroLayer::playStep4)
        );
    }

    auto* delay = cocos2d::CCDelayTime::create(0.0f);
    auto* sequence = cocos2d::CCSequence::create(delay, callFunc, nullptr);
    runAction(sequence);
}

void IntroLayer::playStep4() {
    geode::log::debug("IntroLayer::playStep4()");

    auto* mgr = GameManager::sharedState();
    mgr->m_unkBool11 = false;

    Toolbox::transitionFade(m_playScene);
    CC_SAFE_RELEASE(m_playScene);
}

void IntroLayer::searchFinished(GJGameLevel* level) {
    auto* scene = IntroLayer::scene(level);
    Toolbox::transitionFade(scene);
}

void IntroLayer::searchFailed() {
    auto* alert = FLAlertLayer::create(
        this,
        "Error",
        "Level search failed. Returning to main menu.",
        "OK",
        nullptr
    );

    alert->setTag(Alert::Error);
    alert->show();
}

} // namespace slope
