#include "GameOverLayer.hpp"

#include <Geode/loader/Mod.hpp>
#include <fmt/format.h>

#include <Geode/fmod/fmod.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

#include "Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"
#include "Manager.hpp"
#include "PresetSelectLayer.hpp"
#include "Toolbox.hpp"

#include "../plate/GradientBG.hpp"
#include "../plate/Toolbox.hpp"
#include "../plate/SideArt.hpp"

namespace slope {

cocos2d::CCScene* GameOverLayer::scene() {
    auto* scene = cocos2d::CCScene::create();
    auto* layer = GameOverLayer::create();

    scene->addChild(layer);
    return scene;
}

GameOverLayer* GameOverLayer::create() {
    auto* ret = new GameOverLayer();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool GameOverLayer::init() {
    if (!cocos2d::CCLayer::init()) {
        return false;
    }

    setTouchEnabled(true);
    setKeyboardEnabled(true);

    setTouchMode(cocos2d::kCCTouchesOneByOne); // ???

    auto size = getContentSize();

    auto* background = plate::GradientBG::create({ 32, 32, 32 });
    addChild(background);

    auto* sideArt = plate::SideArt::create("gauntletCorner_001.png");
    sideArt->setColor({ 76, 76, 76 });
    addChild(sideArt);

    auto* gameOver = plate::Toolbox::createSprite("gameOver_001.png"_spr);
    gameOver->setPosition({
        size.width / 2.0f,
        (size.height / 2.0f) + 20.0f
    });
    addChild(gameOver);

    int numLevels = slope::Manager::sharedManager()->getNumLevels();
    auto* label = cocos2d::CCLabelBMFont::create(
        fmt::format("Levels: {}", numLevels).c_str(),
        "goldFont.fnt"
    );
    label->setPosition(
        (size.width / 2.0f),
        (size.height / 2.0f) - 10.0f
    );
    addChild(label);

    return true;
}

void GameOverLayer::onEnterTransitionDidFinish() {
    auto* audioEngine = FMODAudioEngine::sharedEngine();
    audioEngine->playEffect("gameOver.ogg"_spr, 1.0f, 0.0f, 0.7f);

    m_canExit = true;
}

bool GameOverLayer::ccTouchBegan(
    cocos2d::CCTouch* touch,
    cocos2d::CCEvent* event
) {
    exit();
    return true;
}

void GameOverLayer::keyDown(cocos2d::enumKeyCodes key) {
    exit();
}

void GameOverLayer::exit() {
    if (!m_canExit) {
        return;
    }

    auto* scene = PresetSelectLayer::scene();
    Toolbox::transitionFade(scene);
}
    
} // namespace slope
