#include "bench_api.h"

Suite *
bench_api_suite(void)
{
	Suite *s = NULL;
	TCase *tc_core = NULL;

	s = suite_create("bench_api");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, BENCH_ALLOCATE_SMALL);
	tcase_add_test(tc_core, BENCH_ALLOCATE_MEDIUM);

	suite_add_tcase(s, tc_core);

	return s;
}
