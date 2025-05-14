//
// Created by Gordon on 2024/11/12.
//
#include "filepath.h"

const std::string MOUNT_NAME = "/sd";
const std::string STORE_DIR = "store";
const std::string CONFIG_DIR = "config";
const std::string DEBUG_DIR = "debug";
#if DEBUG_ENABLE
const std::string REPO_DIR = "repoT";
#else
const std::string REPO_DIR = "repo";
#endif
const std::string SOUL_DIR = path({REPO_DIR, "soul"});
const std::string ASSETS_DIR = path({REPO_DIR, "assets"});
const std::string DATABASE_DIR = "database";


const std::string REPO_DIFF_FILENAME = "diff";