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

#ifndef LILI_H
#define LILI_H

/*
************************************************************************************************************************
*       INCLUDE FILES
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*       MACROS
************************************************************************************************************************
*/

#define LILI_FOREACH(list, var) \
    for(node_t *var = list->first; var; var = var->next)


/*
************************************************************************************************************************
*       CONFIGURATION
************************************************************************************************************************
*/

#define LILI_NO_DYNAMIC_ALLOCATION
#define LILI_MAX_LISTS      10
#define LILI_MAX_NODES      100


/*
************************************************************************************************************************
*       DATA TYPES
************************************************************************************************************************
*/

typedef struct node_t {
    struct node_t *next;
    struct node_t *prev;
    void *value;
} node_t;

typedef struct lili_t {
    int count;
    node_t *first;
    node_t *last;
} lili_t;


/*
************************************************************************************************************************
*       FUNCTION PROTOTYPES
************************************************************************************************************************
*/

lili_t *lili_create(void);
void lili_destroy(lili_t *list);
void lili_push(lili_t *list, void *value);
void *lili_pop(lili_t *list);


/*
************************************************************************************************************************
*   CONFIGURATION ERRORS
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*   END HEADER
************************************************************************************************************************
*/

#endif
