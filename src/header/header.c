#include "header.h"

// droping const qualifiers leads to undefined behavior,
// interpreting the value on the fly doesnt
typedef union shared_mem {
	char *c_arr;
	const void *v_ptr;
	header_t *hdr;
} shared_chars;

u16
size_of_header(void)
{
	return sizeof(header_t);
}

size_t
minimum(const size_t size)
{
	return size > alignment ? size : alignment;
}

size_t
base_split_limit(const size_t size)
{
	return minimum(0) + size_of_header() + size;
}

header_t *
retrieve_the_header(const void *const ptr)
{
	static shared_chars s;
	s.v_ptr = ptr;
	return (header_t *)s.c_arr - 1;
}

void *
hide_the_header(const void *const ptr)
{
	static shared_chars s;
	s.v_ptr = ptr;
	return (void *)((header_t *)s.c_arr + 1);
}

size_t
adapt_requested_size(const size_t size)
{
	return (size < alignment) ?
	    minimum(size) :
	    (unsigned long)minimum(size) + (minimum(0) - size % minimum(0));
}

header_t *
next_block(const header_t *const block)
{
	static shared_chars s;
	s.v_ptr = block;
	return (header_t *)((char *)s.c_arr + size_of_header() + block->size);
}

header_t *
previous_block(const header_t *const block)
{
	static shared_chars s;
	s.v_ptr = block;
	return (header_t *)((char *)s.c_arr - size_of_header() - block->size);
}

header_t *
split(header_t *block, const u16 requested_size)
{
	header_t *splitted = NULL, *aft = NULL;
	u16 remaining_size = block->size - size_of_header() - requested_size;

	block->size = (u16)requested_size;
	splitted = next_block(block);
	splitted->block_size = block->size;
	splitted->block_free = block->free;
	splitted->size = remaining_size;
	splitted->free = 1;

	aft = next_block(splitted);
	aft->block_size = splitted->size;

	return splitted;
}
