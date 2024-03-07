#include <Geode/modify/CreatorLayer.hpp>

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <cocos2d.h>

#include "../slope/PresetSelectLayer.hpp"
#include "../slope/Toolbox.hpp"

#include "../plate/Toolbox.hpp"

class $modify(CreatorLayer_M, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) {
            return false;
        }

        auto* menu = static_cast<cocos2d::CCMenu*>(
            getChildByIDRecursive("bottom-left-menu")
        );

        auto* sprite = plate::Toolbox::createSprite(
            "challengeIcon_001.png"_spr
        );
        auto* button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(CreatorLayer_M::onSlope)
        );

        menu->addChild(button);
        menu->updateLayout();

        // Reorder bottom left menu above side art.
        menu->setZOrder(2);

        return true;
    }

    void onSlope(cocos2d::CCObject* sender) {
        auto* scene = slope::PresetSelectLayer::scene();
        slope::Toolbox::transitionFade(scene);
    }
};
