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

START_TEST(it_makes_multiple_allocations)
{
    s_allocator_t *stack;
    char *a;
    char *b;

    stack = s_alloc_init(2);
    a = s_alloc(stack, 1, 0);
    b = s_alloc(stack, 1, 0);

    memset(a, 0xFF, 1);
    memset(b, 0xEF, 1);

    ck_assert_int_eq(*((unsigned char *)a), 0xFF);
    ck_assert_int_eq(*((unsigned char *)b), 0xEF);
}
END_TEST

START_TEST(it_rolls_back_and_reallocates)
{
    s_allocator_t *stack;
    char *a;
    char *b;
    char *c;
    char *d;

    stack = s_alloc_init(2);
    a = s_alloc(stack, 1, 0);
    b = s_alloc(stack, 1, 0);

    memset(a, 0x00, 1);
    memset(b, 0x01, 1);

    // free
    s_free(stack, a);

    c = s_alloc(stack, 1, 0);
    d = s_alloc(stack, 1, 0);

    memset(c, 0x02, 1);
    memset(d, 0x03, 1);

    ck_assert_int_eq(*((unsigned char *)c), 0x02);
    ck_assert_int_eq(*((unsigned char *)d), 0x03);
    ck_assert_int_eq(*((unsigned char *)a), *((unsigned char *)c));
    ck_assert_int_eq(*((unsigned char *)b), *((unsigned char *)d));
}
END_TEST

Suite *make_memory_allocator_unit_test_suite()
{
    Suite *s;
    TCase *tc;

    s = suite_create("Memory Allocator");
    tc = tcase_create("Core");

    tcase_add_test(tc, it_allocates_a_single_byte_and_stores_0xFF);
    tcase_add_test(tc, it_makes_multiple_allocations);
    tcase_add_test(tc, it_rolls_back_and_reallocates);

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
