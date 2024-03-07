#include "PresetSelectLayer.hpp"

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CreatorLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Log.hpp>

#include "Preset.hpp"
#include "PresetNode.hpp"
#include "Manager.hpp"
#include "WorldLayer.hpp"
#include "Toolbox.hpp"

#include "../plate/BackButton.hpp"
#include "../plate/GradientBG.hpp"
#include "../plate/SideArt.hpp"
#include "../plate/Toolbox.hpp"

namespace slope {

cocos2d::CCScene* PresetSelectLayer::scene() {
    auto* scene = cocos2d::CCScene::create();
    auto* layer = PresetSelectLayer::create();

    scene->addChild(layer);
    return scene;
}

PresetSelectLayer* PresetSelectLayer::create() {
    auto* ret = new PresetSelectLayer();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool PresetSelectLayer::init() {
    if (!cocos2d::CCLayer::init()) {
        return false;
    }

    setKeypadEnabled(true);
    setTouchEnabled(true);

    auto* background = plate::GradientBG::create();
    addChild(background);

    auto* sideArt = plate::SideArt::create("GJ_sideArt_001.png");
    addChild(sideArt);

    auto* backButton = plate::BackButton::create(
        this,
        menu_selector(PresetSelectLayer::onBack)
    );
    addChild(backButton);

    auto size = getContentSize();
    auto* logo = plate::Toolbox::createSprite("challengeLogo_001.png"_spr);
    logo->setPosition({
        size.width / 2.0f,
        size.height - 40.0f
    });
    addChild(logo);

    buildPresets();
    Manager::sharedManager()->fadeInMusic();

    return true;
}

void PresetSelectLayer::buildPresets() {
    using namespace geode::utils;

    auto* menu = cocos2d::CCMenu::create();
    auto size = getContentSize();
    menu->ignoreAnchorPointForPosition(false);
    menu->setContentSize(size * 0.85f);
    menu->setPosition({
        size.width / 2.0f,
        (size.height / 2.0f) - 20.0f
    });

    // I hate doing this.
    auto* fileUtils = cocos2d::CCFileUtils::sharedFileUtils();
    auto path = fileUtils->fullPathForFilename(
        "default-presets.json"_spr,
        false
    );
    auto presets = file::readJson(path.c_str()).unwrap();
    
    for (auto json : presets["presets"].as_array()) {
        auto* preset = Preset::create(json);
        auto* node = PresetNode::create(preset);
        auto* menuItem = CCMenuItemSpriteExtra::create(
            node,
            this,
            menu_selector(PresetSelectLayer::onSelect)
        );
        menuItem->m_scaleMultiplier = 1.05f;

        menuItem->setUserObject(preset);
        menu->addChild(menuItem);
    }
    
    auto* layout = cocos2d::RowLayout::create();
    menu->setLayout(layout);

    addChild(menu);
}

void PresetSelectLayer::keyBackClicked() {
    onBack(nullptr);
}

void PresetSelectLayer::onBack(cocos2d::CCObject* sender) {
    GameManager::sharedState()->fadeInMenuMusic();

    auto* scene = CreatorLayer::scene();
    Toolbox::transitionFade(scene);
}

void PresetSelectLayer::onSelect(cocos2d::CCObject* sender) {
    auto* menuItem = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto* preset = static_cast<Preset*>(menuItem->getUserObject());

    Manager::sharedManager()->startGame(preset);

    auto* scene = WorldLayer::scene();
    Toolbox::transitionFade(scene);
}

} // namespace slope
