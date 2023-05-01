#pragma once

#include "../header/header.h"

typedef struct memory_module {
	header_t *arena, *free_list;
} memory_module_t;

// Initialise the heap
void initialise(memory_module_t *);

// Return a proper block from the current memory module
header_t *find_suitable_block(memory_module_t *, const size_t);

// Insert a block to the
void insert(memory_module_t *, header_t *);

header_t *merge(memory_module_t *module, header_t *block);
