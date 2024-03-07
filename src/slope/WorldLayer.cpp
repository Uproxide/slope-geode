#include "WorldLayer.hpp"

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <fmt/format.h>

#include <Geode/fmod/fmod.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

#include "IntroLayer.hpp"
#include "Manager.hpp"
#include "PresetSelectLayer.hpp"
#include "Toolbox.hpp"

#include "../plate/BackButton.hpp"
#include "../plate/SideArt.hpp"
#include "../plate/GradientBG.hpp"
#include "../plate/Toolbox.hpp"

namespace slope {

WorldLayer::~WorldLayer() {
    CC_SAFE_RELEASE(m_level);
}

cocos2d::CCScene* WorldLayer::scene() {
    auto* scene = cocos2d::CCScene::create();
    auto* layer = WorldLayer::create();
    
    scene->addChild(layer);
    return scene;
}

WorldLayer* WorldLayer::create() {
    auto* ret = new WorldLayer();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool WorldLayer::init() {
    if (!cocos2d::CCLayer::init()) {
        return false;
    }

    setKeypadEnabled(true);
    setTouchEnabled(true);

    auto* mgr = Manager::sharedManager();
    auto size = getContentSize();

    auto* background = plate::GradientBG::create();
    addChild(background);

    auto* sideArt = plate::SideArt::create("GJ_sideArt_001.png");
    addChild(sideArt);

    auto* backButton = plate::BackButton::create(
        this,
        menu_selector(WorldLayer::onBack)
    );
    addChild(backButton);

    auto* menu = cocos2d::CCMenu::create();

    m_playButton = CCMenuItemSpriteExtra::create(
        plate::Toolbox::createSprite("GJ_playBtn2_001.png"),
        nullptr,
        this,
        menu_selector(WorldLayer::onPlay)
    );
    
    menu->setContentSize(m_playButton->getContentSize());
    m_playButton->setPosition(menu->getContentSize() / 2.0f);
    m_playButton->setVisible(false);

    menu->setPosition(getContentSize() / 2.0f);
    menu->ignoreAnchorPointForPosition(false);
    menu->addChild(m_playButton);
    addChild(menu);

    auto* livesLabel = cocos2d::CCLabelBMFont::create(
        fmt::format("Lives: {}", mgr->getNumLives()).c_str(),
        "goldFont.fnt"
    );
    livesLabel->setPosition({
        size.width / 2.0f,
        (size.height / 2.0f) + 100.0f,
    });
    addChild(livesLabel);

    auto* levelsLabel = cocos2d::CCLabelBMFont::create(
        fmt::format("Levels: {}", mgr->getNumLevels()).c_str(),
        "goldFont.fnt"
    );
    levelsLabel->setPosition({
        size.width / 2.0f,
        (size.height / 2.0f) + 80.0f
    });
    addChild(levelsLabel);

    m_loadingCircle = plate::LoadingCircle::create();
    m_loadingCircle->setPosition(getContentSize() / 2.0f);
    m_loadingCircle->show();
    addChild(m_loadingCircle);
    
    Manager::sharedManager()->fadeInMusic();

    return true;
}

void WorldLayer::keyBackClicked() {
    onBack(nullptr);
}

void WorldLayer::onEnterTransitionDidFinish() {
    auto* searcher = Manager::sharedManager()->getLevelSearcher();
    
    searcher->setDelegate(this);
    searcher->search();
}

void WorldLayer::FLAlert_Clicked(FLAlertLayer* alert, bool second) {
    switch (alert->getTag()) {
    case Alert::Error:
        onQuit(true);
        break;
    
    case Alert::Quit:
        onQuit(second);
        break;
    }
}

void WorldLayer::searchFinished(GJGameLevel* level) {
    m_loadingCircle->fadeAndRemove();
    m_playButton->setVisible(true);

    m_level = level;
    CC_SAFE_RETAIN(m_level);
}

void WorldLayer::searchFailed() {
    m_loadingCircle->fadeAndRemove();

    auto* alert = FLAlertLayer::create(
        this,
        "Error",
        "Search failed. Returning to main menu.",
        "OK",
        ""
    );
    alert->setTag(Alert::Error);
    alert->show();
}

void WorldLayer::onBack(cocos2d::CCObject* sender) {
    int numLevels = Manager::sharedManager()->getNumLevels();

    if (numLevels == 0) {
        onQuit(true);
        return;
    }

    auto* alert = FLAlertLayer::create(
        this,
        "Warning",
        "Are you sure you want to quit?",
        "No",
        "Yes"
    );
    alert->setTag(Alert::Quit);
    alert->show();
}

void WorldLayer::onPlay(cocos2d::CCObject* sender) {
    auto* audioEngine = FMODAudioEngine::sharedEngine();

    audioEngine->stopAllMusic();
    audioEngine->playEffect("playSound_01.ogg", 1.0f, 0.0f, 0.3f);

    auto* scene = IntroLayer::scene(m_level);
    Toolbox::transitionFade(scene);
}

void WorldLayer::onQuit(bool yes) {
    if (!yes) {
        return;
    }

    Manager::sharedManager()->endGame();

    auto* scene = PresetSelectLayer::scene();
    Toolbox::transitionFade(scene);
}

} // namespace slope