#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

#include "../src/stack_allocator.h"

START_TEST(it_allocates_a_single_byte_and_stores_0xFF)
{
    s_allocator_t *stack;
    char * s;

    stack = s_alloc_init(1);
    // 8-bit character can be aligned to any address
    s = s_alloc(stack, 1, 1);

    memset(s, 0xFF, 1);

    ck_assert_int_eq(*((unsigned char *)s), 0xFF);
}
END_TEST

START_TEST(it_makes_multiple_allocations)
{
    s_allocator_t *stack;
    char *a;
    char *b;

    stack = s_alloc_init(4);
    a = s_alloc(stack, 1, 1);
    b = s_alloc(stack, 1, 1);

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

    stack = s_alloc_init(4);
    a = s_alloc(stack, 1, 1);
    b = s_alloc(stack, 1, 1);

    memset(a, 0x00, 1);
    memset(b, 0x01, 1);

    // free
    s_free(stack, a);

    c = s_alloc(stack, 1, 1);
    d = s_alloc(stack, 1, 1);

    memset(c, 0x02, 1);
    memset(d, 0x03, 1);

    ck_assert_int_eq(*((unsigned char *)c), 0x02);
    ck_assert_int_eq(*((unsigned char *)d), 0x03);
    ck_assert_int_eq(*((unsigned char *)a), *((unsigned char *)c));
    ck_assert_int_eq(*((unsigned char *)b), *((unsigned char *)d));
}
END_TEST

START_TEST(it_properly_aligns_allocated_memory)
{
    s_allocator_t *stack;
    char *a;
    uint32_t *b;

    stack = s_alloc_init(16);
    a = s_alloc(stack, 1, 1);
    b = s_alloc(stack, 4, 4);

    *a = 0xFF;
    *b = 0xFFFFFFFF;

    ck_assert_int_eq(*((unsigned char *)a), 0xFF);
    // should be 4 byte aligned
    ck_assert_int_eq(*((uintptr_t *)&b) % 4, 0);
}
END_TEST

START_TEST(it_rejects_alignments_larger_than_128)
{
    s_allocator_t *stack;
    stack = s_alloc_init(16);
    s_alloc(stack, 1, 256);
}
END_TEST

START_TEST(it_rejects_alignments_smaller_than_1)
{
    s_allocator_t *stack;
    stack = s_alloc_init(16);
    s_alloc(stack, 1, 0);
}
END_TEST

START_TEST(alignment_must_be_power_of_two)
{
    s_allocator_t *stack;
    stack = s_alloc_init(16);
    s_alloc(stack, 1, 5);
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
    tcase_add_test(tc, it_properly_aligns_allocated_memory);

    tcase_add_test_raise_signal(tc, it_rejects_alignments_larger_than_128, SIGABRT);
    tcase_add_test_raise_signal(tc, it_rejects_alignments_smaller_than_1, SIGABRT);
    tcase_add_test_raise_signal(tc, alignment_must_be_power_of_two, SIGABRT);

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
