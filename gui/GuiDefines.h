//
// Created by Huwensong on 2025/5/13.
//

#ifndef SIMULATOR_GUIDEFINES_H
#define SIMULATOR_GUIDEFINES_H

#include <any>
#include <memory>
#include <functional>



namespace Gui
{
    using GuiObj = std::any;

    using GuiDismissCb = std::function<void()>;
}

enum InputEvent
{
    BtnSrcLeft = 0,
    BtnSrcRight,
    BtnSrcBack,
    BtnSrcFront,
    BtnSrcPlant,
};

enum InputType {
    ButtonPress,
    ButtonRelease,
    ButtonClick,
    ButtonClickSure,
    ButtonLongPress,
    ButtonLongHolding,
    ButtonMultiple,
};

struct InputData{
    InputType inputType;
    uint8_t n;
};

struct InputEvtType
{
    InputEvent index;
    InputData  data;
};

class GuiDefines
{
public:
    //region Singleton
    static GuiDefines &instance() {
        static std::shared_ptr<GuiDefines> instance(new GuiDefines());
        return *instance;
    }

    GuiDefines(const GuiDefines &) = delete;

    GuiDefines(GuiDefines &&) = delete;

    GuiDefines &operator=(const GuiDefines &) = delete;

    GuiDefines &operator=(GuiDefines &&) = delete;
    //endregion

public:
    GuiDefines() = default;
};


#endif //SIMULATOR_GUIDEFINES_H
