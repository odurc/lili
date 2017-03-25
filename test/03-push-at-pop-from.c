#include <stdio.h>
#include <assert.h>
#include "lili.h"

#define NODES_COUNT 5

int test_nodes(lili_t *list, const int *expected)
{
    int i = 0;
    LILI_FOREACH(list, node)
    {
        int *value = (int *) node->data;
        if (*value != expected[i++])
            return 0;
    }

    return 1;
}

int main(void)
{
    lili_t *list = lili_create();
    int data[NODES_COUNT];

    // push some data
    for (int i = 0; i < NODES_COUNT; ++i)
    {
        data[i] = i;
        lili_push(list, &data[i]);
    }

    printf("current nodes (count = %i):\n", list->count);

    for (node_t *node = list->first; node; node = node->next)
    {
        int *value = (int *) node->data;
        printf("  data: %i\n", *value);
    }

    assert(test_nodes(list, (const int []){0, 1, 2, 3, 4}));

    int more_data[] = {10, 20, 30, 40, 50, 60};

    lili_push_at(list, &more_data[0], 4);
    assert(test_nodes(list, (const int []){0, 1, 2, 3, 10, 4}));

    lili_push_at(list, &more_data[1], 25);
    assert(test_nodes(list, (const int []){0, 1, 2, 3, 10, 4, 20}));

    lili_push_at(list, &more_data[2], 2);
    assert(test_nodes(list, (const int []){0, 1, 30, 2, 3, 10, 4, 20}));

    lili_push_at(list, &more_data[3], -2);
    assert(test_nodes(list, (const int []){0, 1, 30, 2, 3, 10, 4, 40, 20}));

    lili_push_at(list, &more_data[4], -23);
    assert(test_nodes(list, (const int []){50, 0, 1, 30, 2, 3, 10, 4, 40, 20}));

    printf("nodes after push_at (count = %i):\n", list->count);

    for (node_t *node = list->first; node; node = node->next)
    {
        int *value = (int *) node->data;
        printf("  data: %i\n", *value);
    }

    lili_pop_from(list, 0);
    assert(test_nodes(list, (const int []){0, 1, 30, 2, 3, 10, 4, 40, 20}));

    lili_pop_from(list, -1);
    assert(test_nodes(list, (const int []){0, 1, 30, 2, 3, 10, 4, 40}));

    lili_pop_from(list, 100);
    assert(test_nodes(list, (const int []){0, 1, 30, 2, 3, 10, 4}));

    lili_pop_from(list, -50);
    assert(test_nodes(list, (const int []){1, 30, 2, 3, 10, 4}));

    lili_pop_from(list, 4);
    assert(test_nodes(list, (const int []){1, 30, 2, 3, 4}));

    lili_pop_from(list, -3);
    assert(test_nodes(list, (const int []){1, 30, 3, 4}));

    printf("nodes after pop_from (count = %i):\n", list->count);

    for (node_t *node = list->first; node; node = node->next)
    {
        int *value = (int *) node->data;
        printf("  data: %i\n", *value);
    }

    lili_destroy(list);

    return 0;
}
