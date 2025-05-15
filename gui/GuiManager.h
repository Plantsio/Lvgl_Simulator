//
// Created by Huwensong on 2025/5/13.
//

#ifndef SIMULATOR_GUIMANAGER_H
#define SIMULATOR_GUIMANAGER_H

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

class GuiManager
{
public:
    //region Singleton
    static GuiManager &instance() {
        static std::shared_ptr<GuiManager> instance(new GuiManager());
        return *instance;
    }

    GuiManager(const GuiManager &) = delete;

    GuiManager(GuiManager &&) = delete;

    GuiManager &operator=(const GuiManager &) = delete;

    GuiManager &operator=(GuiManager &&) = delete;
    //endregion

public:
    GuiManager() = default;
};


#endif //SIMULATOR_GUIMANAGER_H
