#pragma once

#include <sys/mman.h>

#define NULL ((void *)0)

typedef unsigned short u16;
typedef unsigned long size_t;

// alignment depends on the arch, 64 bit-> 8 bytes
extern const u16 alignment;

extern const size_t heap;

typedef struct header {
	u16 block_free; // Is the previous block free?
	u16 block_size; // Size of the previous block
	u16 free;	// Is this block free?
	u16 size;	// Size of the current block

	struct header *next_block, *previous_block;

} header_t;

// The size of the header struct, exactly 24 bytes
u16 size_of_header(void);

// The minimum size we can allocate apart from the header
size_t minimum(const size_t);

// minimum size of the block we will allocate in order to be split in at least 2
// blocks, smallest splittable block, we need to allocate at least 40 bytes
// (40+24 = 2 * 32 = 24 + 8 + 24 + 8)
size_t base_split_limit(const size_t);

// retrieve the header
header_t *retrieve_the_header(const void *const);

// Get the actual memory available
void *hide_the_header(const void *const);

// Adapts the requested memory size to be aligned
size_t adapt_requested_size(const size_t);

// Retrieve the next block based on the size of the current block
header_t *next_block(const header_t *const);

// Retrieve the previous block based on the size of the current block
header_t *previous_block(const header_t *const);

// Split the header based on the size requested
header_t *split(header_t *, const u16);
