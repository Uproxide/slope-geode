#include "Preset.hpp"

#include <Geode/loader/Mod.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/string.hpp>
#include <matjson/stl_serialize.hpp>

namespace slope {

Preset* Preset::create(matjson::Value const& json) {
    auto* ret = new Preset();

    if (ret && ret->init(json)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool Preset::init(matjson::Value const& json) {
    using namespace geode::utils;

    m_searchType = SearchType::MostLiked;

    m_name = json["name"].as_string();
    m_color = json["color"].as<cocos2d::ccColor3B>();
    
    // Awesome hack
    m_image = string::replace(
        json["image"].as_string(),
        "${spr}",
        geode::Mod::get()->getID()
    );

    m_difficulty = json["difficulty"].as<std::vector<int>>();
    m_uncompletedFilter = json["uncompleted-filter"].as_bool();

    m_numLives = json["lives"].as_int();

    return true;
}

template<class T>
static std::string ijoin(std::vector<T> vec, std::string sep) {
    using namespace geode::utils;

    std::vector<std::string> svec;

    // ???
    if (vec.empty()) {
        return "-";
    }

    // There's probably a less stupid way to do this.
    for (T item : vec) {
        svec.push_back(std::to_string(int(item)));
    }

    return string::join(svec, sep);
}

GJSearchObject* Preset::getSearchObject(int page) {
    // You aren't supposed to initialize a search object like this.
    auto* searchObj = GJSearchObject::create(m_searchType);

    searchObj->m_difficulty = ijoin(m_difficulty, ",");
    searchObj->m_uncompletedFilter = m_uncompletedFilter;

    //! @todo Make this an option?
    searchObj->m_starFilter = true;

    searchObj->m_page = page;
    return searchObj;
}

} // namespace slope
