#pragma once

#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/LevelManagerDelegate.hpp>
#include <cocos2d.h>

#include <random>

#include "Preset.hpp"

namespace slope {

//! @brief Delegate for LevelSearcher.
class LevelSearcherDelegate {
public:

    //! @brief Called when search finishes.
    //! @param level Level found.
    virtual void searchFinished(GJGameLevel* level) = 0;

    //! @brief Called when search fails.
    virtual void searchFailed() = 0;
};

class LevelSearcher : public cocos2d::CCNode,
                      public LevelManagerDelegate {
public:
    LevelSearcher() :
        m_delegate(nullptr),
        m_setNumPages(false),
        m_justSetNumPages(false) {}
    
    ~LevelSearcher() override;

    static LevelSearcher* create(Preset* preset);
    bool init(Preset* preset);

    //! @brief Start level search.
    void search();

    void setDelegate(LevelSearcherDelegate* delegate) {
        m_delegate = delegate;
    }

    void loadLevelsFinished(
        cocos2d::CCArray* levels, 
        char const* hash
    ) override;

    //! @todo Figure out what p0 is.
    void loadLevelsFailed(char const* p0) override;

    //! @todo Figure out what p1 is.
    void setupPageInfo(gd::string pageStr, char const* p1) override;

private:
    //! @brief The weird bit at the end of the search object.
    struct PageInfo {
        PageInfo(std::string str);

        int count;
        int offset;
        int pageSize;
    };

    //! @brief Get a random page.
    //! @return Page number.
    int getPage();

    GameLevelManager* m_mgr;
    LevelSearcherDelegate* m_delegate;

    Preset* m_preset;
    std::mt19937 m_random;

    int m_numPages;
    bool m_setNumPages;

    //! Hack to prevent loadLevelsFinished from being called twice.
    bool m_justSetNumPages;
};

} // namespace slope
