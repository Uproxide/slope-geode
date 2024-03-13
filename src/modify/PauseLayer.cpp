#include <Geode/modify/PauseLayer.hpp>

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <cocos2d.h>

#include <Geode/fmod/fmod.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

#include "../slope/PresetSelectLayer.hpp"
#include "../slope/IntroLayer.hpp"
#include "../slope/Manager.hpp"
#include "../slope/LevelSearcher.hpp"
#include "../slope/Toolbox.hpp"

#include "../plate/Toolbox.hpp"

class Skipper : public slope::LevelSearcherDelegate,
                public FLAlertLayerProtocol {
public:
    Skipper() : m_searching(false) {}

    void searchFinished(GJGameLevel* level) override {
        auto* scene = slope::IntroLayer::scene(level);
        slope::Toolbox::transitionFade(scene);
    }

    void searchFailed() override {
        auto* alert = FLAlertLayer::create(
            this,
            "Error",
            "Level search failed. Returning to main menu.",
            "OK",
            ""
        );
    }

    void FLAlert_Clicked(FLAlertLayer* alert, bool second) override {
        auto* scene = slope::PresetSelectLayer::scene();
        slope::Toolbox::transitionFade(scene);
    }

    void skip() {
        if (m_searching) {
            return;
        }

        auto* mgr = slope::Manager::sharedManager();
        auto* searcher = mgr->getLevelSearcher();
        
        searcher->setDelegate(this);
        searcher->search();

        m_searching = true;
    }

    void reset() {
        m_searching = false;
    }

    bool getSearching() const { return m_searching; }

private:
    bool m_searching;
};

// This is a global variable to prevent a crash. I am lazy.
Skipper g_skipper;

class $modify(PauseLayer_M, PauseLayer) {
    static void onModify(auto& self) {
        // Attempt to load this hook after all the other ones.
        (void)self.setHookPriority("PauseLayer::customSetup", -1);
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto* mgr = slope::Manager::sharedManager();
        if (mgr->getInGame()) {
            customSetupSlope();
        }

        g_skipper.reset();
    }

    void customSetupSlope() {
        auto* menu = static_cast<cocos2d::CCMenu*>(
            getChildByID("center-button-menu")
        );

        auto* skipSprite = plate::Toolbox::createSprite(
            "skipBtn_001.png"_spr
        );
        auto* skipButton = CCMenuItemSpriteExtra::create(
            skipSprite,
            this,
            menu_selector(PauseLayer_M::onSkip)
        );

        menu->insertBefore(skipButton, nullptr);

        // Fixes a compatibility issue where trying to save and play a level in
        // the editor causes a crash. You aren't supposed to be able to open
        // that, so we disable the button for it.
        auto* editButton = menu->getChildByID("edit-button");
        editButton->setVisible(false);
        
        auto* layout = cocos2d::RowLayout::create();
        layout->ignoreInvisibleChildren(true);
        menu->setLayout(layout);
    }

    void onSkip(cocos2d::CCObject* sender) {
        g_skipper.skip();
    }

    void onQuit(cocos2d::CCObject* sender) {
        auto* mgr = slope::Manager::sharedManager();
        if (!mgr->getInGame()) {
            PauseLayer::onQuit(sender);
            return;
        }

        // decrementForcePrio();
        unscheduleAllSelectors();

        auto* audioEngine = FMODAudioEngine::sharedEngine();
        audioEngine->clearAllAudio();
        audioEngine->m_system->update();

        audioEngine->unloadAllEffects();
        audioEngine->playEffect(
            "quitSound_01.ogg",
            1.0f,
            0.0f,
            0.7f
        );

        slope::Manager::sharedManager()->endGame();
        auto* scene = slope::PresetSelectLayer::scene();
        slope::Toolbox::transitionFade(scene);
    }
};
