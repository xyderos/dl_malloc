#include <stdio.h>

#include "../header/header.h"
#include "api.h"

static memory_module_t module;

static void init(void) __attribute__((constructor));
void
init(void)
{
	initialise(&module);
}

static void *
internal_dalloc(memory_module_t *mod, const size_t size)
{
	size_t adapted = adapt_requested_size(size);

	header_t *block = find_suitable_block(mod, adapted);

	if (!block) {
		return NULL;
	}
	return hide_the_header(block);
}

static void
internal_free(memory_module_t *mod, const void *const pointer)
{
	header_t *block = NULL, *aft = NULL;
	if (!pointer) {
		return;
	}

	block = (header_t *)retrieve_the_header(pointer);

	block = merge(&module, block);

	aft = next_block(pointer);

	block->free = 1;

	aft->block_free = 1;

	insert(mod, block);
}

void *
allocate(const size_t size)
{
	return internal_dalloc(&module, size);
}

void
deallocate(const void *const pointer)
{
	internal_free(&module, pointer);
}

#ifdef DEBUG
memory_module_t *

get_mod(void)
{
	return &module;
}
#endif
