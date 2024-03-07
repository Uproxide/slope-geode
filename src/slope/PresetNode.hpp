#pragma once

#include <cocos2d.h>
#include "Preset.hpp"

namespace slope {

//! @brief Preset node for the preset select layer. This will probably be
//!        removed in a future version.
class PresetNode : public cocos2d::CCNode {
public:
    ~PresetNode();

    //! @brief Create preset node.
    //! @param preset Preset.
    //! @return Preset node.
    static PresetNode* create(Preset* preset);

    //! @brief Initialize preset node.
    //! @param preset Preset.
    //! @return True on success, false on failure.
    bool init(Preset* preset);

    Preset* getPreset() const { return m_preset; }

private:
    Preset* m_preset;
};

} // namespace slope
