//
// Created by Gordon on 2024/6/25.
//

#ifndef FIRMWARE_FILEPATH_H
#define FIRMWARE_FILEPATH_H

#include <variant>
#include <string>

/* default dirs */
extern const std::string MOUNT_NAME;
extern const std::string STORE_DIR;
extern const std::string CONFIG_DIR;
extern const std::string DEBUG_DIR;
extern const std::string REPO_DIR;
extern const std::string SOUL_DIR;
extern const std::string ASSETS_DIR;
extern const std::string DATABASE_DIR;

/* ensured dirs */
#define ENSURED_DIRS    {STORE_DIR, CONFIG_DIR, DEBUG_DIR, REPO_DIR, SOUL_DIR, ASSETS_DIR,DATABASE_DIR}

extern const std::string REPO_DIFF_FILENAME;

std::string path(std::initializer_list<std::variant<std::string, const char*>> paths);

#endif //FIRMWARE_FILEPATH_H
