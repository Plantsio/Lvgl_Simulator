//
// Created by sungaoran on 2025/7/17.
//

#include "Indicators.h"

namespace UI {
    Indicators::Indicators(lv_obj_t *parent) {
        for (int i = 0; i < IndPartAll; i++) {
            m_indicators.emplace_back(parent);
        }
        m_indicators[IndPartLeft].align(LV_ALIGN_LEFT_MID, 3, 0);
        m_indicators[IndPartRight].align(LV_ALIGN_RIGHT_MID, -3, 0);
    }

    void Indicators::setState(Indicators::IndPart part, IndicatorBase::IndState state) {
        std::vector<IndicatorBase *> indParts;
        if (part == IndPartAll) {
            for (int i = 0; i < IndPartAll; i++) {
                indParts.push_back(&m_indicators[i]);
            }
        } else {
            indParts.push_back(&m_indicators[part]);
        }

        for (auto ind: indParts) {
            ind->setState(state);
        }
    }
}
