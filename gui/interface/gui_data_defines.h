//
// Created by Huwensong on 2025/1/20.
//

#ifndef IVY2_0_GUI_DATA_DEFINES_H
#define IVY2_0_GUI_DATA_DEFINES_H

#include <string>
#include <functional>

namespace Gui {
    struct UIDataBasic{
        bool horizontalRoutable;
    };

    struct DataActionBox {
        std::string title{};
        std::string description{};
        std::string applyDesc{};
        std::string cancelDesc{};
        std::function<void()> applyCB{};
        std::function<void()> cancelCB{};
        std::function<void()> finalCB{};
        int countdownTimeout{};
    };

//    struct ProvInfo
//    {
//        IOTManager::NetStatus status;
//        uint32_t errCode;
//    };

//    using DataPlayer = VideoGroup;
//
//    using ObjData = std::variant<std::monostate, DataPlayer, DataActionBox, bool, ProvInfo>; //todo:为什么不是 std::any,这样多一种数据就得加一个类型，每个UI都得使用std::visit

//    struct ObjData : std::variant<std::monostate, DataPlayer, DataActionBox> {
//        using variant::variant;
//
//        [[nodiscard]] constexpr bool isNull() const noexcept {
//            return std::holds_alternative<std::monostate>(*this);
//        }
//    };
}

#endif //IVY2_0_GUI_DATA_DEFINES_H

