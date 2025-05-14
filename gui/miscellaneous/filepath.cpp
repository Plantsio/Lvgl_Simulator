//
// Created by Gordon on 2024/11/12.
//
#include "filepath.h"
#include <sstream>

const std::string MOUNT_NAME = "./";
const std::string STORE_DIR = "store";
const std::string CONFIG_DIR = "config";
const std::string DEBUG_DIR = "debug";
const std::string REPO_DIR = "resources";
const std::string SOUL_DIR = path({REPO_DIR, "soul"});
const std::string ASSETS_DIR = path({REPO_DIR, "assets"});
const std::string DATABASE_DIR = "database";


const std::string REPO_DIFF_FILENAME = "diff";

std::string path(std::initializer_list<std::variant<std::string, const char*>> paths) {
    std::ostringstream ret;
    ret << MOUNT_NAME; // Assuming MOUNT_NAME is defined somewhere
    for (const auto &p : paths) {
        std::string pathStr;
        if (std::holds_alternative<std::string>(p)) {
            pathStr = std::get<std::string>(p);
        } else if (std::holds_alternative<const char*>(p)) {
            pathStr = std::get<const char*>(p);
        }

        // Remove "/sd" from the beginning of the path if it exists
        if (pathStr.find(MOUNT_NAME) == 0) {
            pathStr.erase(0, MOUNT_NAME.length()); // Remove the first 3 characters ("/sd")
        }

        // Append the path to the result
        ret << "/" << pathStr;
    }
    return ret.str();
}