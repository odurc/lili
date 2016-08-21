#include <stdio.h>
#include "lili.h"

int main(void)
{
    lili_t *list = lili_create();

    lili_push(list, "one");
    lili_push(list, "two");
    lili_push(list, "three");
    printf("%s\n", (char *) lili_pop(list));
    printf("%s\n", (char *) lili_pop(list));
    printf("%s\n", (char *) lili_pop(list));

    lili_destroy(list);

    return 0;
}
