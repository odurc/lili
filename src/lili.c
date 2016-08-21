/*
 * Copyright (c) 2016 Ricardo Crudo <ricardo.crudo@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
************************************************************************************************************************
*       INCLUDE FILES
************************************************************************************************************************
*/

#include "lili.h"


/*
************************************************************************************************************************
*       INTERNAL MACROS
************************************************************************************************************************
*/

// if alternative malloc/free is not defined then use functions from libc
#if !defined(LILI_NO_DYNAMIC_ALLOCATION) && !defined(MALLOC)
#include <stdlib.h>
#define MALLOC  malloc
#define FREE    free
#endif

#define INIT_LIST(list) if (list) {list->count = 0; list->first = 0; list->last = 0;}
#define INIT_NODE(node) if (node) {node->next = 0; node->prev = 0; node->value = 0;}


/*
************************************************************************************************************************
*       INTERNAL CONSTANTS
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*       INTERNAL DATA TYPES
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*       INTERNAL GLOBAL VARIABLES
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*       INTERNAL FUNCTIONS
************************************************************************************************************************
*/

static void *remove_node(lili_t *list, node_t *node)
{
    if (!node)
        return 0;

    if (node == list->first && node == list->last)
    {
        list->first = 0;
        list->last = 0;
    }
    else if (node == list->first)
    {
        list->first = node->next;
        list->first->prev = 0;
    }
    else if (node == list->last)
    {
        list->last = node->prev;
        list->last->next = 0;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    list->count--;
    void *value = node->value;

    FREE(node);

    return value;
}


/*
************************************************************************************************************************
*       GLOBAL FUNCTIONS
************************************************************************************************************************
*/

lili_t *lili_create(void)
{
    lili_t *list = MALLOC(sizeof (lili_t));
    INIT_LIST(list);
    return list;
}

void lili_destroy(lili_t *list)
{
    LILI_FOREACH(list, node)
    {
        if (node->prev)
            FREE(node->prev);
    }

    FREE(list->last);
    FREE(list);
}

void lili_push(lili_t *list, void *value)
{
    node_t *node = MALLOC(sizeof (node_t));

    if (!node)
        return;

    INIT_NODE(node);

    node->value = value;

    if (list->last)
    {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }
    else
    {
        list->first = node;
        list->last = node;
    }

    list->count++;
}

void *lili_pop(lili_t *list)
{
    return remove_node(list, list->last);
}
