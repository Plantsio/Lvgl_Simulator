//
// Created by Gordon on 2022/5/26.
//

#ifndef PLANTSIO_IVY_UIBOOT_H
#define PLANTSIO_IVY_UIBOOT_H

#include <functional>

#include "UIBase.h"
#include "Progress.h"



namespace UI {
    class UIBoot : public Base
    {
    public:
        explicit UIBoot(ObjPtr obj);

    public:
        static void set(const std::string &desc, int progress);

        void update_now(const std::string &desc, int progress);

        bool check_routine(const std::function<bool()> &check_cb, int progress, int timeout, const std::string &success_desc,
						   const std::string &fail_desc);

        static int get_current_progress();

        static void set_current_progress(int progress);

        static std::string get_current_desc();

        static void set_current_desc(const std::string &desc);

        static void timer_cb(lv_timer_t *timer);

        void next();

    private:
        void refresh_timeout();

        bool _initialize() override;

        bool _deInitialize() override;

    private:
        lv_timer_t *m_timer = nullptr;
        int m_current_step = 0;
        uint32_t m_current_start_t = 0;

        uint32_t m_plant_detect_cnt = 0;

        bool over_time = false;

        Progress mProcess;
    };
}


#endif //PLANTSIO_IVY_UIBOOT_H
