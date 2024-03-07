#pragma once

#include <cocos2d.h>
#include "Preset.hpp"
#include "LevelSearcher.hpp"

namespace slope {

class Manager : public cocos2d::CCObject {
public:
    static Manager* sharedManager();

    void startGame(Preset* preset);
    void endGame();

    void fadeInMusic();

    auto* getLevelSearcher() const { return m_levelSearcher; }

    bool getInGame() const { return m_inGame; }

    int getNumLives() const { return m_numLives; }
    void setNumLives(int numLives) { m_numLives = numLives; }

    int getNumLevels() const { return m_numLevels; }
    void setNumLevels(int numLevels) { m_numLevels = numLevels; } 

private:
    static Manager* s_sharedManager;

    Preset* m_preset;
    LevelSearcher* m_levelSearcher;

    bool m_inGame;
    int m_numLives;
    int m_numLevels;
};

} // namespace slope