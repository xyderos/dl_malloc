#include <stddef.h>

#include "../header/header.h"
#include "memory_module.h"

const u16 alignment = 8;
const size_t heap = (size_t)64 * 1024;

// Initialize the heap
void
initialise(memory_module_t *module)
{
	size_t size = 0;
	header_t *new_block = NULL, *sentinel = NULL;

	new_block = mmap(NULL, heap, PROT_READ | PROT_WRITE,
	    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (!new_block) {
		return;
	}

	size = heap - (unsigned long)(2 * size_of_header());

	new_block->block_free = 0;
	new_block->block_size = 0;
	new_block->free = 1;
	new_block->size = (u16)size;

	sentinel = next_block(new_block);

	sentinel->block_free = new_block->free;
	sentinel->free = 0;

	module->arena = new_block;
	module->free_list = new_block;
}

// Detach a block from the free list of the heap
static void
detach(memory_module_t *module, header_t *block)
{
	if (block->next_block) {
		block->next_block->previous_block = block->previous_block;
	}

	if (block->previous_block) {
		block->previous_block->next_block = block->next_block;

	} else {
		module->free_list = block->next_block;
	}

	block->next_block = NULL;
	block->previous_block = NULL;
}

// Insert a block in the free list of the heap
void
insert(memory_module_t *module, header_t *block)
{
	block->next_block = module->free_list;
	block->previous_block = NULL;

	if (module->free_list) {
		module->free_list->previous_block = block;
	}

	module->free_list = block;
}

// Find the next block available from the free list based on the size requested
header_t *
find_suitable_block(memory_module_t *module, const size_t requested_size)
{
	// iterator on the first element of the free list
	header_t *iterator = module->free_list;

	while (iterator) {
		if (iterator->size >= requested_size) {
			if (iterator->size >=
			    base_split_limit(requested_size)) {
				header_t *off = split(iterator,
				    (u16)requested_size);
				iterator->free = 0;
				next_block(iterator)->block_free = 0;

				detach(module, iterator);
				insert(module, off);

				return iterator;
			} else {
				iterator->free = 0;
				next_block(iterator)->block_free = 0;

				detach(module, iterator);

				return iterator;
			}
		}
		iterator = iterator->next_block;
	}
	return NULL;
}

header_t *
merge(memory_module_t *module, header_t *block)
{
	u16 new_size = 0;
	// get the 2 blocks adjacent to out current block
	header_t *n = next_block(block), *bef = NULL, *nn = NULL;

	if (block->block_free) {
		bef = previous_block(block);

		detach(module, bef);

		new_size = block->size + bef->size + size_of_header();
		n->block_size = new_size;
		bef->size = new_size;
		block = bef;
	}

	if (n->free) {
		detach(module, n);

		new_size = block->size + n->size + size_of_header();
		nn = next_block(n);

		nn->block_size = new_size;
		block->size = new_size;
	}
	return block;
}
