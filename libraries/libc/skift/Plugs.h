#pragma once
#include <libsystem/Common.h>

/* --- Memory allocator ----------------------------------------------------- */

void __plug_memory_lock();

void __plug_memory_unlock();

void *__plug_memory_alloc(size_t size);

void __plug_memory_free(void *address, size_t size);