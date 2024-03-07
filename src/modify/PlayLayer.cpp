#include <Geode/modify/PlayLayer.hpp>

#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Log.hpp>

#include <Geode/fmod/fmod.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

#include "../slope/Manager.hpp"
#include "../slope/LivesNode.hpp"
#include "../slope/GameOverLayer.hpp"
#include "../slope/Toolbox.hpp"

class $modify(PlayLayer_M, PlayLayer) {
    bool init(GJGameLevel* level, bool p1, bool p2) {
        if (!PlayLayer::init(level, p1, p2)) {
            return false;
        }

        auto* mgr = slope::Manager::sharedManager();
        if (mgr->getInGame()) {
            initSlope();
        }
        
        return true;
    }

    void initSlope() {
        geode::log::debug("PlayLayer::initSlope()");

        m_fields->m_livesNode = slope::LivesNode::create();
        auto size = getContentSize();

        m_fields->m_livesNode->setPosition({
            45.0f,
            size.height - 25.0f
        });
        addChild(m_fields->m_livesNode);

        m_fields->m_firstAttempt = true;
    }

    void levelComplete() {
        auto* mgr = slope::Manager::sharedManager();
        PlayLayer::levelComplete();

        if (!mgr->getInGame()) {
            return;
        }

        int numLevels = mgr->getNumLevels();
        mgr->setNumLevels(numLevels + 1);
    }

    void resetLevel() {
        auto* mgr = slope::Manager::sharedManager();

        if (!mgr->getInGame()) {
            PlayLayer::resetLevel();
            return;
        }

        updateLives();

        if (mgr->getNumLives() <= 0) {
            auto* sequence = cocos2d::CCSequence::create(
                cocos2d::CCDelayTime::create(0.5f),
                cocos2d::CCCallFunc::create(
                    this,
                    callfunc_selector(PlayLayer_M::gameOver)
                ),
                nullptr
            );

            runAction(sequence);
        } else {
            PlayLayer::resetLevel();
        }
    }

    void updateLives() {
        auto* mgr = slope::Manager::sharedManager();

        if (!mgr->getInGame()) {
            return;
        }

        if (m_fields->m_firstAttempt) {
            m_fields->m_firstAttempt = false;
            return;
        }
        
        int numLives = mgr->getNumLives();
        numLives -= 1;
        mgr->setNumLives(numLives);

        if (numLives > 0) {
            m_fields->m_livesNode->updateLabel();
        }
    }

    void gameOver() {
        auto* mgr = slope::Manager::sharedManager();
        auto* audioEngine = FMODAudioEngine::sharedEngine();
        audioEngine->clearAllAudio();

        mgr->endGame();

        auto* scene = slope::GameOverLayer::scene();
        slope::Toolbox::transitionFade(scene);
    }

    slope::LivesNode* m_livesNode;
    bool m_firstAttempt;
};
