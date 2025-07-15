//
// Created by Gordon on 2022/5/26.
//

#ifndef PLANTSIO_IVY_UIBOOT_H
#define PLANTSIO_IVY_UIBOOT_H

#include <functional>

#include "UIBase.h"
#include "components/Step/StepHandler.h"
#include "Progress.h"


namespace UI {
    class UIBoot : public Base
    {
    public:
        explicit UIBoot(ObjPtr obj);

    private:
        using ConditionCb = std::function<bool()>;

    private:
        bool _initialize() override;

        void update_now(const std::string &desc, int progress);

        void registerStep(ConditionCb condition,uint32_t conditionTimeout,const std::string &successDsc,const std::string &failDsc,int failToJump = STEP_FAIL_NO_JUMP);

    private:
        StepHandler mStepHandler;
        Progress mProcess;
    };
}


#endif //PLANTSIO_IVY_UIBOOT_H
