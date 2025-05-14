//
// Created by Gordon on 2024/7/10.
//

#ifndef FIRMWARE_DUMP_H
#define FIRMWARE_DUMP_H

#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include "tools/path.h"
#include "sys_config.h"

/** note: should only be used for temp storage if data type can change.
 * @tparam T data type struct
 */
template<typename T>
class Dump {
public:
    explicit Dump(std::string filepath, T defaultValue = T{})
            : m_path(std::move(filepath)),
              m_cache(std::move(defaultValue)) {
    }

public:
    bool save() {
        std::ofstream outFile(m_path, std::ios::binary);
        log_v("Saving dump %s", m_path.c_str());
        if (outFile.is_open()) {
            if constexpr (std::is_same<T, std::string>::value) {
                // Handle std::string serialization
                size_t length = m_cache.size();
                outFile.write(reinterpret_cast<const char *>(&length), sizeof(length)); // Write string length
                outFile.write(m_cache.data(), length); // Write string content
            } else {
                outFile.write(reinterpret_cast<const char *>(&m_cache), sizeof(T));
            }
            outFile.close();
            log_d("Dump %s successfully saved", m_path.c_str());
            return true;
        } else {
            log_d("failed to open dump %s", m_path.c_str());
            return false;
        }
    }

    bool load(bool autoCreate = true) {
        auto fileSize = getFileSize(m_path);

        if (fileSize == -1) {
            log_d("File %s not found", m_path.c_str());
            if (autoCreate) {
                return save();
            }
            return false;
        }

        // Handle both non-std::string and std::string types
        if constexpr (std::is_same<T, std::string>::value) {
            // For std::string, we first read the length and then the content
            size_t length;
            std::ifstream inFile(m_path, std::ios::binary);
            if (inFile.is_open()) {
                inFile.read(reinterpret_cast<char *>(&length), sizeof(length)); // Read string length
                m_cache.resize(length); // Resize the string to fit the content
                inFile.read(&m_cache[0], length); // Read the string content
                log_d("Dump %s successfully loaded", m_path.c_str());
                inFile.close();
                return true;
            } else {
                // create a new one
                if (autoCreate) {
                    return save();
                }
                return false;
            }
        } else {
            // For non-std::string types, we check the file size to match the expected data size
            if (fileSize != sizeof(T)) {
                log_w("Dump %s wrong size %d | %d", m_path.c_str(), sizeof(T), (int) fileSize);
                return false;
            }
            std::ifstream inFile(m_path, std::ios::binary);
            if (inFile.is_open()) {
                inFile.read(reinterpret_cast<char *>(&m_cache), sizeof(T));
                log_d("Dump %s successfully loaded", m_path.c_str());
                inFile.close();
                return true;
            } else {
                // create a new one
                if (autoCreate) {
                    return save();
                }
                return false;
            }
        }
    }

    bool remove() {
        if (std::remove(m_path.c_str()) == 0) {
            log_d("Dump %s successfully removed", m_path.c_str());
            return true;
        } else {
            log_d("Failed to remove dump %s", m_path.c_str());
            return false;
        }
    }

    T &data() {
        return m_cache;
    }

    const std::string &getPath() {
        return m_path;
    }

protected:
    std::string m_path;
    T m_cache;
};


#endif //FIRMWARE_DUMP_H
