#include <check.h>
#include <limits.h>
#include <string.h>

#include "../../src/api/api.h"

Suite *test_api_suite(void);

START_TEST(TEST_INT_PTR_ALLOCATE_AND_DEALLOCATE)
{
	int *mem = allocate(sizeof(int));

	*mem = 1;

	ck_assert_int_eq(*mem, 1);

	deallocate(mem);
}
END_TEST

START_TEST(TEST_CHAR_PTR_ALLOCATE_AND_DEALLOCATE)
{
	char *mem = allocate(sizeof(char) * 16);
	strcpy(mem, "kostas");

	ck_assert_str_eq(mem, "kostas");

	deallocate(mem);
}
END_TEST

START_TEST(TEST_STRUCT_PTR_ALLOCATE_AND_DEALLOCATE)
{
	struct trash {
		int x;
		double y;
		char z;
		struct trash *next;
	};

	struct trash *mem = allocate(sizeof(struct trash));
	ck_assert_ptr_nonnull(mem);

	mem->x = 1;
	mem->y = 1.0;
	mem->z = 'K';

	ck_assert_int_eq(mem->x, 1);
	ck_assert_double_eq(mem->y, 1.0);
	ck_assert_int_eq(mem->z, 'K');

	deallocate(mem);
}
END_TEST

START_TEST(COMPLEX_TEST_STRUCT_PTR_ALLOCATE_AND_DEALLOCATE)
{
	struct trash {
		int x;
		double y;
		char *z;
		struct trash *next;
	};

	struct trash *mem = allocate(sizeof(struct trash));
	ck_assert_ptr_nonnull(mem);

	mem->x = 1;
	mem->y = 1.0;
	mem->z = allocate(sizeof(char) * 16);
	mem->z = "kostas";

	ck_assert_int_eq(mem->x, 1);
	ck_assert_double_eq(mem->y, 1.0);
	ck_assert_str_eq(mem->z, "kostas");

	deallocate(mem);
}
END_TEST
