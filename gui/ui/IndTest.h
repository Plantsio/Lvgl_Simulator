//
// Created by sungaoran on 2025/7/17.
//

#ifndef SIMULATOR_INDTEST_H
#define SIMULATOR_INDTEST_H

#include "UIBase.h"
#include "Indicators.h"

namespace UI {

    class IndTest : public Base {
    public:
        IndTest();

    private:
        bool _initialize() override;

        static void test(lv_event_t*e);

    private:
        lv_obj_t *m_label;
        Indicators m_indicators;
    };
}


#endif //SIMULATOR_INDTEST_H
