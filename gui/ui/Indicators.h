//
// Created by sungaoran on 2025/7/17.
//

#ifndef SIMULATOR_INDICATORS_H
#define SIMULATOR_INDICATORS_H

#include "lvgl.h"
#include "vector"
#include "IndicatorBase.h"

namespace UI {
    class Indicators {
    public:
        enum IndPart {
            IndPartLeft,
            IndPartRight,
            IndPartAll,
        };

    public:
        explicit Indicators(lv_obj_t *parent);

    public:
        void setState(IndPart part, IndicatorBase::IndState state);


    private:
        std::vector<IndicatorBase> m_indicators;

    };
}

#endif //SIMULATOR_INDICATORS_H
