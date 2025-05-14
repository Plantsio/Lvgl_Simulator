//
// Created by Gordon on 2024/9/23.
//

#ifndef FIRMWARE_DEINITIALIZABLE_H
#define FIRMWARE_DEINITIALIZABLE_H

#include "Initializable.h"

class DeInitializable : public Initializable {
public:
    virtual bool deInitialize() {
        if (!m_initialized) {
            return true;
        }
        m_initialized = _deInitialize();
        return m_initialized;
    }

    virtual bool _deInitialize() = 0;

};


#endif //FIRMWARE_DEINITIALIZABLE_H
