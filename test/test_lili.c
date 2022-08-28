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
    static int data[10];

    list = lili_create();
    assert_non_null(list);

    // push some data to the list
    for (int i = 0; i < 10; i++)
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
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
