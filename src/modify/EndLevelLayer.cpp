#include <Geode/modify/EndLevelLayer.hpp>

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <cocos2d.h>

#include <Geode/fmod/fmod.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

#include "../plate/Toolbox.hpp"

#include "../slope/Manager.hpp"
#include "../slope/Toolbox.hpp"
#include "../slope/WorldLayer.hpp"

class $modify(MyEndLevelLayer, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        if (!inGame()) {
            return;
        }

        // I'm not sure why m_buttonMenu doesn't work.
        auto* buttonMenu = m_mainLayer->getChildByID("button-menu");
        auto* exitButton = buttonMenu->getChildByID("exit-button");
        auto* replayButton = buttonMenu->getChildByID("retry-button");
        auto* leaderboardButton = buttonMenu->getChildByID(
            "leaderboard-button"
        );

        // The leaderboard button leads to a sort of privilege escalation.
        // Disable it. 
        if (leaderboardButton != nullptr) {
            leaderboardButton->setVisible(false);
        }

        if (!canExit()) {
            exitButton->setVisible(false);
        }

        if (!canReplay()) {
            replayButton->setVisible(false);
        }

        // Useless failsafe
        if (m_playLayer->m_level->m_levelType == GJLevelType::Saved) {
            addSideButtons();
        }
    }

    void keyDown(cocos2d::enumKeyCodes key) {
        if (!inGame()) {
            EndLevelLayer::keyDown(key);
            return;
        }

        if (isExitKey(key) && canExit()) {
            onMenu(nullptr);
            return;
        }

        if (isReplayKey(key) && canReplay()) {
            onReplay(nullptr);
            return;
        }

        cocos2d::CCLayer::keyDown(key);
    }

    void onMenu(cocos2d::CCObject* sender) {
        if (!inGame()) {
            EndLevelLayer::onMenu(sender);
            return;
        }

        auto* gameMgr = GameManager::sharedState();
        gameMgr->doQuickSave();

        auto* audioEngine = FMODAudioEngine::sharedEngine();
        audioEngine->clearAllAudio();
        audioEngine->m_system->update(); // ???
        audioEngine->playEffect("quitSound_01.ogg", 1.0f, 0.0f, 0.7f);

        auto* scene = slope::WorldLayer::scene();
        slope::Toolbox::transitionFade(scene);
    }

    void addSideButtons() {
        auto* background = m_mainLayer->getChildByID("background");
        cocos2d::CCPoint center = background->getPosition();
        cocos2d::CCSize size = background->getContentSize();

        auto* menu = cocos2d::CCMenu::create();

        cocos2d::CCPoint offset = {
            size.width - 25.0f,
            size.height / 2.0f
        };

        menu->ignoreAnchorPointForPosition(false);
        menu->setContentSize({
            20.0f,
            size.height * 0.9f
        });
        menu->setPosition(offset + center);
        m_mainLayer->addChild(menu);

        auto* likeSprite = plate::Toolbox::createSprite(
            "GJ_likeBtn_001.png"
        );
        likeSprite->setScale(0.7f);

        auto* dislikeSprite = plate::Toolbox::createSprite(
            "GJ_dislikeBtn_001.png"
        );
        dislikeSprite->setScale(0.7f);

        m_fields->m_likeButton = CCMenuItemSpriteExtra::create(
            likeSprite,
            this,
            menu_selector(MyEndLevelLayer::onLike)
        );

        m_fields->m_dislikeButton = CCMenuItemSpriteExtra::create(
            dislikeSprite,
            this,
            menu_selector(MyEndLevelLayer::onDislike)
        );

        menu->addChild(m_fields->m_dislikeButton);
        menu->addChild(m_fields->m_likeButton);
        
        auto* layout = cocos2d::ColumnLayout::create();
        layout->setAxisAlignment(cocos2d::AxisAlignment::Start);
        menu->setLayout(layout);

        updateSideButtons();
    }

    void updateSideButtons() {
        auto* levelMgr = GameLevelManager::sharedState();
        auto* level = m_playLayer->m_level;

        if (levelMgr->hasLikedItemFullCheck(
            LikeItemType::Level,
            level->m_levelID,
            false // "special"
        )) {
            // Make like buttons grayscale.
            // This is kind of a stupid way to do this.

            m_fields->m_likeButton->setEnabled(false);
            plate::Toolbox::initSprite(
                static_cast<cocos2d::CCSprite*>(
                    m_fields->m_likeButton->getNormalImage()
                ),
                "GJ_likeBtn2_001.png" // Unused?
            );

            m_fields->m_dislikeButton->setEnabled(false);
            plate::Toolbox::initSprite(
                static_cast<cocos2d::CCSprite*>(
                    m_fields->m_dislikeButton->getNormalImage()
                ),
                "GJ_dislikeBtn2_001.png"_spr
            );
        }
    }

    void onLike(cocos2d::CCObject* sender) {
        triggerLike(true);
    }

    void onDislike(cocos2d::CCObject* sender) {
        triggerLike(false);
    }

    void triggerLike(bool like) {
        auto* levelMgr = GameLevelManager::sharedState();
        auto* level = m_playLayer->m_level;

        levelMgr->likeItem(
            LikeItemType::Level,
            level->m_levelID,
            like,
            false // "special"
        );

        updateSideButtons();
    }

    //! @brief Are we doing a challenge run?
    inline bool inGame() const {
        return slope::Manager::sharedManager()->getInGame();
    }

    //! @brief Can we press the exit button?
    inline bool canExit() const {
        return !m_playLayer->m_isPracticeMode;
    }

    //! @brief Can we press the replay button?
    inline bool canReplay() const {
        return m_playLayer->m_isPracticeMode;
    }

    //! @brief Is this one of the exit hotkeys?
    inline bool isExitKey(cocos2d::enumKeyCodes key) const {
        return (
            key == cocos2d::KEY_Escape ||
            key == cocos2d::CONTROLLER_B
        );
    }

    //! @brief Is this one of the replay hotkeys?
    inline bool isReplayKey(cocos2d::enumKeyCodes key) const {
        return (
            key == cocos2d::KEY_Space ||
            key == cocos2d::CONTROLLER_A ||
            key == cocos2d::CONTROLLER_Start
        );
    }

    CCMenuItemSpriteExtra* m_likeButton;
    CCMenuItemSpriteExtra* m_dislikeButton;
};
