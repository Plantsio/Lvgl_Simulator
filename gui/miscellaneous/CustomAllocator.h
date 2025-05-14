

#ifndef FIRMWARE_CUSTOMALLOCATOR_H
#define FIRMWARE_CUSTOMALLOCATOR_H

#include <iostream>
#include <streambuf>
#include <memory>
#include "interface.h"

template <typename T>
class CustomAllocator {
public:
	using value_type = T;

	CustomAllocator() noexcept = default;

	template <class U>
	explicit CustomAllocator(const CustomAllocator<U>&) noexcept {}

	T* allocate(std::size_t n) {
		// Custom allocation logic can go here
		T* ptr = static_cast<T*>(heap_caps_malloc(n * sizeof(T),MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
		if (!ptr) {
			throw std::bad_alloc();
		}
		return ptr;
	}

	void deallocate(T* p, std::size_t n) noexcept {
		// Custom deallocation logic can go here
		free(p);
	}
};

// Compare two custom allocators - typically always equal unless they manage different resources
template <typename T, typename U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&) { return true; }

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&) { return false; }

#endif //FIRMWARE_CUSTOMALLOCATOR_H