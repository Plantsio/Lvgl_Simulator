//
// Created by Huwensong on 2025/5/14.
//

#ifndef SIMULATOR_INTERFACE_H
#define SIMULATOR_INTERFACE_H

#include <cstdio>

#define MALLOC_CAP_SPIRAM 0
#define MALLOC_CAP_8BIT   0

void *heap_caps_malloc(size_t size, char caps);

#endif //SIMULATOR_INTERFACE_H
