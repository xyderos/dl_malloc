#include "test_api.h"

Suite *
test_api_suite(void)
{
	Suite *s = NULL;
	TCase *tc_core = NULL;

	s = suite_create("test_api");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, TEST_INT_PTR_ALLOCATE_AND_DEALLOCATE);
	tcase_add_test(tc_core, TEST_CHAR_PTR_ALLOCATE_AND_DEALLOCATE);
	tcase_add_test(tc_core, TEST_STRUCT_PTR_ALLOCATE_AND_DEALLOCATE);
	tcase_add_test(tc_core,
	    COMPLEX_TEST_STRUCT_PTR_ALLOCATE_AND_DEALLOCATE);

	suite_add_tcase(s, tc_core);

	return s;
}
