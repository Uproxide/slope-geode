#include <Geode/modify/EndLevelLayer.hpp>

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/utils/cocos.hpp>

#include <Geode/fmod/fmod.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

#include "../slope/Manager.hpp"
#include "../slope/WorldLayer.hpp"
#include "../slope/Toolbox.hpp"

class $modify(EndLevelLayer_M, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        auto* mgr = slope::Manager::sharedManager();
        if (mgr->getInGame()) {
            customSetupSlope();
        }
    }

    void customSetupSlope() {
        auto* menuButton = static_cast<CCMenuItemSpriteExtra*>(
            getChildByIDRecursive("exit-button")
        );

        if(m_playLayer->m_isPracticeMode) {
            menuButton->removeFromParentAndCleanup(true);
        }
    }

    void onMenu(cocos2d::CCObject* sender) {
        auto* mgr = slope::Manager::sharedManager();
        
        if (!mgr->getInGame()) {
            EndLevelLayer::onMenu(sender);
            return;
        }

        auto* audioEngine = FMODAudioEngine::sharedEngine();
        audioEngine->clearAllAudio();
        audioEngine->m_system->update();
        
        audioEngine->playEffect(
            "quitSound_01.ogg",
            1.0f,
            0.0f,
            0.7f
        );

        auto* scene = slope::WorldLayer::scene();
        slope::Toolbox::transitionFade(scene);
    }
};
