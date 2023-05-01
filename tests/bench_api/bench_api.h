#include <check.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/api/api.h"

#define MAX 4000
#define LIMIT 100000
#define MIN 8
#define ROUNDS 10
#define LOOP_SMALL 10
#define LOOP_MEDIUM 20
#define BUFFER 10
#define TRASH 1234567890

Suite *bench_api_suite(void);

static int
request(void)
{
	double k = log(((double)MAX) / MIN),
	       r = ((double)(rand() % (int)(k * LIMIT))) / LIMIT;

	return (int)((double)MAX / exp(r));
}

START_TEST(BENCH_ALLOCATE_SMALL)
{
	void *buffer[BUFFER];
	int *memory = NULL, index = 0;

	for (int i = 0; i < BUFFER; i++) {
		buffer[i] = NULL;
	}

	for (int j = 0; j < ROUNDS; j++) {
		for (int i = 0; i < LOOP_SMALL; i++) {

			index = rand() % BUFFER;

			if (buffer[index]) {
				deallocate(buffer[index]);
				buffer[index] = NULL;
			} else {
				memory = allocate((size_t)request());

				ck_assert_ptr_nonnull(memory);

				buffer[index] = memory;

				*memory = TRASH;
			}
		}
	}
}
END_TEST

START_TEST(BENCH_ALLOCATE_MEDIUM)
{
	void *buffer[BUFFER];
	int *memory = NULL, index = 0;

	for (int i = 0; i < BUFFER; i++) {
		buffer[i] = NULL;
	}

	for (int j = 0; j < ROUNDS; j++) {
		for (int i = 0; i < LOOP_MEDIUM; i++) {

			index = rand() % BUFFER;

			if (buffer[index]) {
				deallocate(buffer[index]);
				buffer[index] = NULL;
			} else {
				memory = allocate((size_t)request());

				ck_assert_ptr_nonnull(memory);

				buffer[index] = memory;

				*memory = TRASH;
			}
		}
	}
}
END_TEST
