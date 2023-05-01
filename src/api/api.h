#pragma once

#include "../header/header.h"
#include "../memory_module/memory_module.h"

#define DEBUG

// Allocate the memory requested in bytes
void *allocate(const size_t);

// Deallocate the memory requested via a pointer
void deallocate(const void *const);

#ifdef DEBUG
memory_module_t *get_mod(void);
#endif
