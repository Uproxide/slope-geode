#include "LevelSearcher.hpp"

#include <Geode/loader/Log.hpp>
#include <Geode/utils/string.hpp>

namespace slope {

LevelSearcher::~LevelSearcher() {
    CC_SAFE_RELEASE(m_preset);

    m_mgr->m_levelManagerDelegate = nullptr;
}

LevelSearcher* LevelSearcher::create(Preset* preset) {
    auto* ret = new LevelSearcher();

    if (ret && ret->init(preset)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool LevelSearcher::init(Preset* preset) {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    m_preset = preset;
    CC_SAFE_RETAIN(m_preset);

    std::random_device rd;
    m_random = std::mt19937(rd());

    m_mgr = GameLevelManager::sharedState();
    m_mgr->m_levelManagerDelegate = this; // Why do I do this here?

    return true;
}

void LevelSearcher::search() {
    int page = getPage();
    auto* searchObj = m_preset->getSearchObject(page);

    geode::log::debug("Searching for levels on page {}", page);

    m_mgr->getOnlineLevels(searchObj);
}

void LevelSearcher::loadLevelsFinished(
    cocos2d::CCArray* levels,
    char const* hash
) {
    if (m_justSetNumPages) {
        m_justSetNumPages = false;
        return;
    }

    //! @todo Check if this is a thing that can actually happen.
    if (levels->count() == 0) {
        m_delegate->searchFailed();
    }

    int index = m_random() % levels->count();
    auto* level = static_cast<GJGameLevel*>(
        levels->objectAtIndex(index)
    );

    // Exclude auto levels that can happen randomly sometimes.
    if (level->m_autoLevel) {
        geode::log::debug("Auto level");
        search();

        return;
    }

    // A few deleted levels still appear in search for some reason?
    // Restart search if we happen upon one of them.
    // I was lucky enough to encounter one during testing.
    if (level->m_stars == 0) {
        geode::log::debug("What the fuck?");
        search();

        return;
    }

    geode::log::debug(
        "Found level: {} ({})",
        level->m_levelName,
        level->m_levelID.value()
    );

    m_delegate->searchFinished(level);
}

void LevelSearcher::loadLevelsFailed(char const* p0) {
    m_delegate->searchFailed();
}

void LevelSearcher::setupPageInfo(gd::string pageStr, char const* p1) {
    PageInfo info(pageStr);

    m_numPages = (info.count - 1) / 10;

    if (m_setNumPages) {
        return;
    }

    m_setNumPages = true;
    m_justSetNumPages = true;

    // Retry search.
    search();
}

LevelSearcher::PageInfo::PageInfo(std::string str) {
    using namespace geode::utils;

    auto tokens = string::split(str, ":");

    count = std::stoi(tokens[0]);
    offset = std::stoi(tokens[1]);
    pageSize = std::stoi(tokens[2]);
}

int LevelSearcher::getPage() {
    if (!m_setNumPages) {
        return 0;
    }

    return m_random() % m_numPages;
}

} // namespace slope
