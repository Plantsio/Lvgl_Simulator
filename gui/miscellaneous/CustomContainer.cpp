//
// Created by Gordon on 2025/5/7.
//
#include "CustomContainer.h"

std::string to_std_string(const CustomString& cs) {
    return {cs.data(), cs.size()};
}