#include "Manager.hpp"

#include <Geode/fmod/fmod.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Log.hpp>

namespace slope {

Manager* Manager::s_sharedManager = nullptr;

Manager* Manager::sharedManager() {
    if (s_sharedManager == nullptr) {
        s_sharedManager = new Manager();
    }

    return s_sharedManager;
}

void Manager::startGame(Preset* preset) {
    m_inGame = true;

    CC_SAFE_RELEASE(preset);
    m_preset = preset;
    CC_SAFE_RETAIN(preset);

    m_levelSearcher = LevelSearcher::create(m_preset);
    CC_SAFE_RETAIN(m_levelSearcher);

    m_numLives = preset->getNumLives();
    m_numLevels = 0;
}

void Manager::endGame() {
    m_inGame = false;
    
    CC_SAFE_RELEASE(m_preset);
    CC_SAFE_RELEASE(m_levelSearcher);
}

void Manager::fadeInMusic() {
    geode::log::debug("Manager::fadeInMusic()");

    auto* gameMgr = GameManager::sharedState();
    auto* audioEngine = FMODAudioEngine::sharedEngine();
    auto* fileUtils = cocos2d::CCFileUtils::sharedFileUtils();

    std::string path = fileUtils->fullPathForFilename(
        "challengeLoop.mp3"_spr,
        false
    );

    bool noMusic = gameMgr->getGameVariable("0122");
    if (noMusic) {
        audioEngine->stopAllMusic();
        return;
    }

    if (audioEngine->isMusicPlaying(path, 0)) {
        return;
    }

    audioEngine->playMusic(path, true, 1.0f, 0);
}

} // namespace slope