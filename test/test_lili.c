/*
****************************************************************************************************
*       INCLUDE FILES
****************************************************************************************************
*/

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <cmocka.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "lili.h"

/*
****************************************************************************************************
*       INTERNAL MACROS
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL CONSTANTS
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL DATA TYPES
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL GLOBAL VARIABLES
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL FUNCTIONS
****************************************************************************************************
*/

static int setup(void **state)
{
    static lili_t *list;
    static int data[5];

    list = lili_create();
    assert_non_null(list);

    // push some data to the list
    for (int i = 0; i < 5; i++)
    {
        data[i] = i;
        lili_push(list, &data[i]);
    }

    *state = list;

    return 0;
}

static int teardown(void **state)
{
    lili_t *list = *state;
    lili_clear(list);
    assert_int_equal(list->count, 0);

    lili_destroy(list);

    return 0;
}

static void test_max_config(void **state)
{
    lili_t *lists[LILI_MAX_LISTS];

    // create as much lists as possible
    for (int i = 0; i < LILI_MAX_LISTS; ++i)
    {
        lists[i] = lili_create();
        assert_non_null(lists[i]);
    }

    // should return null as all instances have been used
    assert_null(lili_create());

    // destroy all instances
    for (int i = 0; i < LILI_MAX_LISTS; i++)
    {
        lili_destroy(lists[i]);
    }

    // should be able to create a new list again
    lili_t *list = lili_create();
    assert_non_null(list);

    // push as much nodes as possible
    int data[LILI_MAX_NODES];
    for (int i = 0; i < LILI_MAX_NODES; i++)
    {
        data[i] = i;
        lili_push(list, &data[i]);
    }

    assert_int_equal(list->count, LILI_MAX_NODES);

    // try to push an extra value, nothing should happen
    // list count shouldn't increase
    int value = 1234;
    lili_push(list, &value);
    assert_int_equal(list->count, LILI_MAX_NODES);

    // pop all nodes back and validate values
    for (int i = 0; i < LILI_MAX_NODES; i++)
    {
        int *pvalue, j = LILI_MAX_NODES - i - 1;
        pvalue = (int *) lili_pop(list);
        assert_int_equal(*pvalue, data[j]);
    }

    assert_int_equal(list->count, 0);

    // try to pop another value
    // list should be empty hence return null
    int *pvalue = (int *) lili_pop(list);
    assert_null(pvalue);

    // use all nodes again
    for (int i = 0; i < LILI_MAX_NODES; i++)
    {
        lili_push(list, &value);
    }

    assert_int_equal(list->count, LILI_MAX_NODES);

    // clear list and confirm if it is empty
    lili_clear(list);
    assert_int_equal(list->count, 0);

    lili_destroy(list);
}

static void test_iteration(void **state)
{
    lili_t *list = *state;

    // forward iteration
    int data = 0;
    for (node_t *node = list->first; node; node = node->next, data++)
    {
        int *pvalue = (int *) node->data;
        assert_int_equal(*pvalue, data);
    }

    // backward iteration
    data = list->count - 1;
    for (node_t *node = list->last; node; node = node->prev, data--)
    {
        int *pvalue = (int *) node->data;
        assert_int_equal(*pvalue, data);
    }
}

static bool check_list_values(lili_t *list, const int *expected)
{
    int i = 0;
    LILI_FOREACH(list, node)
    {
        int *value = (int *) node->data;
        if (*value != expected[i++])
            return false;
    }

    return true;
}

static void test_pushes_and_pops(void **state)
{
    lili_t *list = *state;

    int values[] = {10, 20, 30, 40, 50, -100};
    int i = 0;

    // push at valid position (positive value)
    lili_push_at(list, &values[i++], 4);
    assert_true(check_list_values(list, (const int []){0, 1, 2, 3, 10, 4}));

    // push at position out of the range (positive value)
    // value should be added at the last position instead
    lili_push_at(list, &values[i++], 25);
    assert_true(check_list_values(list, (const int []){0, 1, 2, 3, 10, 4, 20}));

    // push at position zero, should behave as list_push_front function
    lili_push_at(list, &values[i++], 0);
    assert_true(check_list_values(list, (const int []){30, 0, 1, 2, 3, 10, 4, 20}));

    // push at valid position (negative value)
    lili_push_at(list, &values[i++], -2);
    assert_true(check_list_values(list, (const int []){30, 0, 1, 2, 3, 10, 4, 40, 20}));

    // push at position out of the range (negative value)
    // value should be added at the first position instead
    lili_push_at(list, &values[i++], -23);
    assert_true(check_list_values(list, (const int []){50, 30, 0, 1, 2, 3, 10, 4, 40, 20}));

    // push front
    lili_push_front(list, &values[i++]);
    assert_true(check_list_values(list, (const int []){-100, 50, 30, 0, 1, 2, 3, 10, 4, 40, 20}));

    // pop front
    lili_pop_front(list);
    assert_true(check_list_values(list, (const int []){50, 30, 0, 1, 2, 3, 10, 4, 40, 20}));

    // pop from position zero, should behave like pop_front function
    lili_pop_from(list, 0);
    assert_true(check_list_values(list, (const int []){30, 0, 1, 2, 3, 10, 4, 40, 20}));

    // pop from valid position (negative value)
    lili_pop_from(list, -1);
    assert_true(check_list_values(list, (const int []){30, 0, 1, 2, 3, 10, 4, 40}));

    // pop from position out of the range (positive value)
    // value should be poped from the last position instead
    lili_pop_from(list, 100);
    assert_true(check_list_values(list, (const int []){30, 0, 1, 2, 3, 10, 4}));

    // pop from position out of the range (negative value)
    // value should be added at the first position instead
    lili_pop_from(list, -50);
    assert_true(check_list_values(list, (const int []){0, 1, 2, 3, 10, 4}));

    // pop from valid position (positive value)
    lili_pop_from(list, 4);
    assert_true(check_list_values(list, (const int []){0, 1, 2, 3, 4}));
}


/*
****************************************************************************************************
*       MAIN FUNCTION
****************************************************************************************************
*/

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_max_config),
        cmocka_unit_test_setup_teardown(test_iteration, setup, teardown),
        cmocka_unit_test_setup_teardown(test_pushes_and_pops, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
