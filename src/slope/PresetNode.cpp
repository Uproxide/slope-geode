#include "PresetNode.hpp"

#include <Geode/ui/TextArea.hpp>
#include <Geode/loader/Log.hpp>
#include <cocos-ext.h>

#include "../plate/Toolbox.hpp"
#include "ccTypes.h"

namespace slope {

PresetNode::~PresetNode() {
    CC_SAFE_RELEASE(m_preset);
}

PresetNode* PresetNode::create(Preset* preset) {
    auto* ret = new PresetNode();

    if (ret && ret->init(preset)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool PresetNode::init(Preset* preset) {
    if (!cocos2d::CCNode::init()) {
        return false;
    }

    m_preset = preset;
    CC_SAFE_RETAIN(m_preset);

    setAnchorPoint({ 0.5f, 0.5f });

    auto* background = cocos2d::extension::CCScale9Sprite::create(
        "GJ_squareB_01.png",
        { 0.0f, 0.0f, 86.0f, 86.0f }
    );

    background->setContentSize({ 100.0f, 224.0f });
    background->setColor(m_preset->getColor());
    addChild(background);

    setContentSize(background->getContentSize());
    background->setPosition(getContentSize() / 2.0f);

    //! @todo This isn't perfectly centered because Geode doesn't strip the
    //!       space at the end, Fuck! Report this.
    auto* label = geode::SimpleTextArea::create(
        m_preset->getName().c_str(),
        "bigFont.fnt",
        0.6f,
        75.0f
    );
    label->setAlignment(cocos2d::kCCTextAlignmentCenter);
    label->setWrappingMode(geode::WORD_WRAP);
    label->setPosition(
        (getContentSize() / 2.0f) + cocos2d::CCPoint(0.0f, 77.0f)
    );
    addChild(label);

    auto* image = plate::Toolbox::createSprite(m_preset->getImage().c_str());
    image->setScale(55.0f / image->getContentSize().width);
    image->setPosition(
        (getContentSize() / 2.0f) + cocos2d::CCPoint(0.0f, -10.0f)
    );
    addChild(image);

    return true;
}

} // namespace slope
