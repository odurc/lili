#include <stdio.h>
#include "lili.h"

#define NODES_COUNT 5

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

    printf("%i nodes pushed to list\n", list->count);
    printf("---\n");

    // forward iteration
    for (node_t *node = list->first; node; node = node->next)
    {
        int *value = (int *) node->data;
        printf("data: %i\n", *value);
    }

    printf("---\n");

    // backward iteration
    for (node_t *node = list->last; node; node = node->prev)
    {
        int *value = (int *) node->data;
        printf("data: %i\n", *value);
    }

    lili_destroy(list);

    return 0;
}
