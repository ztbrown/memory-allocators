#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/stack_allocator.h"

START_TEST(it_allocates_a_single_byte_and_stores_0xFF)
{
    s_allocator_t *stack; 
    char * s; 

    stack = s_alloc_init(1);
    // 8-bit character can be aligned to any address
    s = s_alloc(stack, 1, 0);

    memset(s, 0xFF, 1);

    ck_assert_int_eq(*((unsigned char *)s), 0xFF);
}
END_TEST

Suite *make_memory_allocator_unit_test_suite()
{
    Suite *s;
    TCase *tc;

    s = suite_create("Memory Allocator");
    tc = tcase_create("Core");

    tcase_add_test(tc, it_allocates_a_single_byte_and_stores_0xFF);

    suite_add_tcase(s, tc);

    return s;
}

int main()
{
    int number_failed;
    SRunner *sr;

    sr = srunner_create(make_memory_allocator_unit_test_suite());

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed==0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
