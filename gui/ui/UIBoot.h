//
// Created by Gordon on 2022/5/26.
//

#ifndef PLANTSIO_IVY_UIBOOT_H
#define PLANTSIO_IVY_UIBOOT_H

#include <functional>

#include "UIBase.h"
#include "StepBase.h"
#include "Progress.h"


namespace UI {
    class UIBoot : public Base, public StepBase
    {
    public:
        explicit UIBoot(ObjPtr obj);

    public:
        using ConditionCb = std::function<bool()>;
        using HandleCb = std::function<void()>;

        enum BootStep
        {
            boot_connecting,
            boot_activating,
            boot_tuning_time,
            boot_locating,
            boot_physical_check,
            boot_plant_detect,
            boot_step_num,
        };

        //using ConditionCb = std::function<bool()>;

    public:
        static void set(const std::string &desc, int progress);

        void update_now(const std::string &desc, int progress);

        bool check_routine(const ConditionCb &condition, int progress, int timeout,
                           const HandleCb &success_handler,
						   const HandleCb &fail_handler);

        static int get_current_progress();

        static void set_current_progress(int progress);

        static std::string get_current_desc();

        static void set_current_desc(const std::string &desc);


    private:
        void refresh_timeout();

        bool _initialize() override;

    private:
        int m_current_step = 0;
        uint32_t m_current_start_t = 0;

        uint32_t m_plant_detect_cnt = 0;

        bool over_time = false;

        Progress mProcess;
    };
}


#endif //PLANTSIO_IVY_UIBOOT_H
