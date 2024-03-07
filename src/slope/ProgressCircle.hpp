#pragma once

#include <cocos2d.h>

namespace slope {

//! @brief LevelInfoLayer progress circle recreation.
//! @todo Move this into plate.
class ProgressCircle : public cocos2d::CCNode {
public:
    static ProgressCircle* create();
    bool init() override;

    void setPercentage(float percentage) {
        m_progressTimer->setPercentage(percentage);
    }

private:
    cocos2d::CCProgressTimer* m_progressTimer;
};

} // namespace slope
