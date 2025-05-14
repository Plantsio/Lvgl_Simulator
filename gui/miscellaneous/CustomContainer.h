//
// Created by Huwensong on 2024/10/18.
//

#ifndef FIRMWARE_CUSTOMCONTAINER_H
#define FIRMWARE_CUSTOMCONTAINER_H

#include <unordered_map>
#include "CustomAllocator.h"
//#include "PSRAMBuffer.h"
#include <set>
#include <vector>

//unordered_map
template<typename Key, typename Tp>
using CustomUnorederMap = std::unordered_map<Key, Tp, std::hash<Key>, std::equal_to<Key>, CustomAllocator<std::pair<const Key, Tp>>>;

using CustomString = std::basic_string<char, std::char_traits<char>, CustomAllocator<char>>;

std::string to_std_string(const CustomString& cs);

namespace std {
    template<>
    struct hash<CustomString> {
        size_t operator()(const CustomString& s) const noexcept {
            return std::hash<std::string_view>{}(std::string_view(s.data(), s.size()));
        }
    };
}

template <typename T>
using CustomVector = std::vector<T, CustomAllocator<T>>;

template<typename Key>
using CustomSet = std::set<Key, std::less<Key>, CustomAllocator<Key>>;

using CustomStringStream = std::basic_stringstream<char, std::char_traits<char>, CustomAllocator<char>>;
#endif //FIRMWARE_CUSTOMCONTAINER_H
