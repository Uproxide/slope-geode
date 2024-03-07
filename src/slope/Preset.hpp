#pragma once

#include <Geode/binding/GJSearchObject.hpp>
#include <matjson.hpp>
#include <cocos2d.h>

#include <string>
#include <vector>

namespace slope {

//! @brief Game configuration.
class Preset : public cocos2d::CCObject {
public:
    static Preset* create(matjson::Value const& json);

    //! @details This has no reason to return bool, but I might as well be
    //!          consistent.
    bool init(matjson::Value const& json);

    //! @brief Generate a search object.
    //! @param page Page number.
    //! @return Search object.
    GJSearchObject* getSearchObject(int page);

    auto getName() const { return m_name; }
    auto getImage() const { return m_image; }
    auto getColor() const { return m_color; }

    auto getNumLives() const { return m_numLives; }

private:
    SearchType m_searchType;
    std::vector<int> m_difficulty; //!< @todo Make this an enum.
    bool m_uncompletedFilter;

    std::string m_name;
    std::string m_image;
    cocos2d::ccColor3B m_color;

    int m_numLives;
};

} // namespace slope
