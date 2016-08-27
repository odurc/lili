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

// if configured to not use dynamic allocation then uses internal functions
#ifdef LILI_NO_DYNAMIC_ALLOCATION
#define LIST_ALLOC      list_take
#define LIST_FREE       list_give
#define NODE_ALLOC      node_take
#define NODE_FREE       node_give
// using dynamic allocation
#else
// if user didn't define custom malloc/free functions then uses regular libc functions
#ifndef MALLOC
#include <stdlib.h>
#define MALLOC          malloc
#define FREE            free
#endif
// set macros to use previously defined malloc/free functions
#define LIST_ALLOC      MALLOC
#define LIST_FREE       FREE
#define NODE_ALLOC      MALLOC
#define NODE_FREE       FREE
#endif

#define LIST_INIT(list) if (list) {list->count = 0; list->first = 0; list->last = 0;}
#define NODE_INIT(node) if (node) {node->next = 0; node->prev = 0; node->data = 0;}


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

#ifdef LILI_NO_DYNAMIC_ALLOCATION
static lili_t g_lists_cache[LILI_MAX_LISTS];
static node_t g_nodes_cache[LILI_MAX_NODES];
#endif


/*
************************************************************************************************************************
*       INTERNAL FUNCTIONS
************************************************************************************************************************
*/

#ifdef LILI_NO_DYNAMIC_ALLOCATION
static inline void *list_take(int n)
{
    // unused parameter
    // it's here to make the function prototype compatible with malloc
    (void) n;

    static unsigned int lists_counter;

    // first time lists are requested
    if (lists_counter < LILI_MAX_LISTS)
    {
        lili_t *list = &g_lists_cache[lists_counter++];
        return list;
    }

    // iterate all array searching for a free list
    // a list is considered free when its count is lower than zero
    for (int i = 0; i < LILI_MAX_LISTS; i++)
    {
        lili_t *list = &g_lists_cache[i];
        if (list->count < 0)
        {
            list->count = 0;
            return list;
        }
    }

    return 0;
}

static inline void list_give(void *list)
{
    if (list)
    {
        lili_t *self = list;
        self->count = -1;
    }
}

static inline void *node_take(int n)
{
    // unused parameter
    // it's here to make the function prototype compatible with malloc
    (void) n;

    static unsigned int nodes_counter;

    // first time nodes are requested
    if (nodes_counter < LILI_MAX_NODES)
    {
        node_t *node = &g_nodes_cache[nodes_counter++];
        return node;
    }

    // iterate all array searching for a free node
    // a node is considered free when its value is null
    for (int i = 0; i < LILI_MAX_NODES; i++)
    {
        node_t *node = &g_nodes_cache[i];
        if (node->data == 0)
            return node;
    }

    return 0;
}

static inline void node_give(void *node)
{
    if (node)
    {
        node_t *self = node;
        self->data = 0;
    }
}
#endif

static void *node_remove(lili_t *list, node_t *node)
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
    void *value = node->data;

    NODE_FREE(node);

    return value;
}


/*
************************************************************************************************************************
*       GLOBAL FUNCTIONS
************************************************************************************************************************
*/

lili_t *lili_create(void)
{
    lili_t *list = LIST_ALLOC(sizeof (lili_t));
    LIST_INIT(list);
    return list;
}

void lili_destroy(lili_t *list)
{
    LILI_FOREACH(list, node)
    {
        if (node->prev)
            NODE_FREE(node->prev);
    }

    NODE_FREE(list->last);
    LIST_FREE(list);
}

void lili_push(lili_t *list, void *data)
{
    node_t *node = NODE_ALLOC(sizeof (node_t));

    if (!node)
        return;

    NODE_INIT(node);

    node->data = data;

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
    return node_remove(list, list->last);
}
