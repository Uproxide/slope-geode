#include "AssetDownloader.hpp"

#include <Geode/loader/Log.hpp>
#include <Geode/utils/string.hpp>

namespace slope {

AssetDownloader::~AssetDownloader() {
    m_mgr->removeMusicDownloadDelegate(this);
}

AssetDownloader* AssetDownloader::create(GJGameLevel* level) {
    auto* ret = new AssetDownloader();

    if (ret && ret->init(level)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool AssetDownloader::init(GJGameLevel* level) {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    parse(level->m_songID, level->m_songIDs, level->m_sfxIDs);

    m_mgr = MusicDownloadManager::sharedState();
    m_mgr->addMusicDownloadDelegate(this);

    return true;
}

void AssetDownloader::loadSongInfoFinished(SongInfoObject* info) {
    m_mgr->downloadSong(info->m_songID);
}

void AssetDownloader::loadSongInfoFailed(int id, GJSongError err) {
    m_failed = true;
    downloadNextAsset();
}

void AssetDownloader::downloadSongFinished(int id) {
    downloadNextAsset();
}

void AssetDownloader::downloadSongFailed(int id, GJSongError err) {
    m_failed = true;
    downloadNextAsset();
}

void AssetDownloader::downloadSFXFinished(int id) {
    downloadNextAsset();
}

void AssetDownloader::downloadSFXFailed(int id, GJSongError err) {
    m_failed = true;
    downloadNextAsset();
}

void AssetDownloader::musicActionFinished(GJMusicAction action) {
    // 2 is the action for getting the custom content URL.
    if (static_cast<int>(action) != 2) {
        return;
    }

    // Resume downloading assets normally.
    downloadNextAsset();
}

void AssetDownloader::musicActionFailed(GJMusicAction action) {
    // 2 is the action for getting the custom content URL.
    if (static_cast<int>(action) != 2) {
        return;
    }

    // The game never call a callback if the custom content URL isn't set.
    // Abort downloading all assets.
    m_delegate->assetDownloadFailed();
}

void AssetDownloader::parse(
    int songID,
    std::string songStr,
    std::string sfxStr
) {
    using namespace geode::utils;

    // Why can't things be simple?
    if (songStr.empty() && sfxStr.empty() && songID != 0) {
        m_assets.emplace(AssetType::Song, songID);
        return;
    }

    for (auto token : string::split(songStr, ",")) {
        int id = std::stoi(token);
        m_assets.emplace(AssetType::Song, id);
    }

    for (auto token : string::split(sfxStr, ",")) {
        int id = std::stoi(token);
        m_assets.emplace(AssetType::SFX, id);
    }
}

void AssetDownloader::finish() {
    if (m_failed) {
        m_delegate->assetDownloadFailed();
    } else {
        m_delegate->assetDownloadFinished();
    }
}

void AssetDownloader::downloadNextAsset() {
    // Custom content URL is required to download some assets.
    if (m_mgr->m_customContentURL.empty()) {
        geode::log::debug("Download custom content URL");
        m_mgr->getCustomContentURL();
        return;
    }

    geode::log::debug("Assets remaining: {}", m_assets.size());

    if (m_assets.empty()) {
        finish();
        return;
    }

    Asset& asset = m_assets.front();
    m_assets.pop();

    switch (asset.type) {
    case AssetType::Song:
        downloadSong(asset.id);
        break;
    case AssetType::SFX:
        downloadSFX(asset.id);
        break;
    }
}

void AssetDownloader::downloadSong(int id) {
    geode::log::debug("Download song: {}", id);

    if (m_mgr->isSongDownloaded(id)) {
        downloadNextAsset();
        return;
    }

    // CustomSongWidget does this, for some reason.
    if (id < 10000000) {
        m_mgr->getSongInfo(id, false);
    } else {
        m_mgr->downloadSong(id);
    }
}

void AssetDownloader::downloadSFX(int id) {
    geode::log::debug("Download SFX: {}", id);

    if (m_mgr->isSFXDownloaded(id)) {
        downloadNextAsset();
        return;   
    }

    m_mgr->downloadSFX(id);
}

} // namespace slope
