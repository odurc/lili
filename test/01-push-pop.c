#include <stdio.h>
#include "lili.h"

#define NODES_COUNT 100

int main(void)
{
    lili_t *list = lili_create();
    int data[NODES_COUNT];

    // lili push
    for (int i = 0; i < NODES_COUNT; ++i)
    {
        data[i] = i;
        lili_push(list, &data[i]);
    }

    // lili pop
    for (int i = 0; i < NODES_COUNT; ++i)
    {
        int *value, j = NODES_COUNT - i - 1;
        value = lili_pop(list);
        if (*value != data[j])
        {
            printf("read wrong value from node (i = %i)\n", i);
            printf("expected: %i, read: %i\n", data[i], *value);
            return -1;
        }
    }

    lili_destroy(list);

    return 0;
}
