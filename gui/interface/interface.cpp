//
// Created by Huwensong on 2025/5/14.
//

#include "interface.h"
#include <cstdlib>

void *heap_caps_malloc(size_t size, char caps)
{
    return malloc(size);
}
