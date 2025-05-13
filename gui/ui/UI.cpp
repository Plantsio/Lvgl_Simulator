//
// Created by Gordon on 2022/11/25.
//

#include "UI.h"

namespace UI {
    std::unordered_map<int, UIBuilder> ui_builders = {};

    std::unordered_map<int, UIBuilder> &get_builders() {
        return ui_builders;
    }
}