//
// Created by Gordon on 2024/7/16.
//

#ifndef FIRMWARE_INITIALIZABLE_H
#define FIRMWARE_INITIALIZABLE_H

#include <cinttypes>

class Initializable {
public:
    Initializable() = default;

    virtual ~Initializable() = default;

public:
    bool initialize() {
        if (m_initialized) {
            return true;
        }
        m_initialized = _initialize();
        return m_initialized;
    }

    virtual bool _initialize() = 0;

    [[nodiscard]] bool initialized() const {
        return m_initialized;
    }

protected:
    bool m_initialized = false;
};


#endif //FIRMWARE_INITIALIZABLE_H
