#include <stdio.h>
#include <assert.h>
#include "lili.h"

#define NODES_COUNT 5

void print_list(lili_t *list)
{
    for (node_t *node = list->first; node; node = node->next)
    {
        int *value = (int *) node->data;
        printf("data: %i\n", *value);
    }

    printf("---\n");

    printf("list size: %i\n", list->count);
    printf("---\n");
}

int main(void)
{
    lili_t *list = lili_create();
    int data[NODES_COUNT];

    // push some data
    printf("adding data to list\n");
    for (int i = 0; i < NODES_COUNT; ++i)
    {
        data[i] = i;
        lili_push(list, &data[i]);
    }

    print_list(list);

    // clear list
    printf("cleaning the list\n");
    lili_clear(list);

    // count must be zero and list still existing
    assert(list->count == 0);
    assert(list != 0);

    printf("list size: %i\n", list->count);
    printf("---\n");

    // push data again
    printf("adding data to list\n");
    for (int i = 0; i < NODES_COUNT; ++i)
    {
        data[i] = NODES_COUNT - i;
        lili_push(list, &data[i]);
    }

    print_list(list);

    lili_destroy(list);

    return 0;
}
