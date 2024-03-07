#pragma once

#include <cocos2d.h>

namespace slope {

class LivesNode : public cocos2d::CCNode {
public:
    static LivesNode* create();
    bool init() override;

    void updateLabel();

private:
    cocos2d::CCLabelBMFont* m_label;
};

} // namespace slope