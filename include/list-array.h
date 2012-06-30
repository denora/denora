/* Macros to handle insertion, deletion, iteration, and searching for
 * linked lists and arrays.
 *
 * (c) 2004-2012 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of
 * IRC Services is copyright (c) 1996-2006 Andrew Church.
 *     E-mail: <achurch@achurch.org>
 * Parts written by Andrew Kempe and others.
 *
 *
 *
 */

#ifndef LIST_ARRAY_H
#define LIST_ARRAY_H

/*************************************************************************/

/* Remove anything defined by system headers. */

#undef LIST_INSERT
#undef LIST_INSERT_ORDERED
#undef LIST_REMOVE
#undef LIST_FOREACH
#undef LIST_FOREACH_SAFE
#undef LIST_SEARCH
#undef LIST_SEARCH_SCALAR
#undef LIST_SEARCH_ORDERED
#undef LIST_SEARCH_ORDERED_SCALAR
#undef ARRAY2_EXTEND
#undef ARRAY2_INSERT
#undef ARRAY2_REMOVE
#undef ARRAY2_FOREACH
#undef ARRAY2_SEARCH
#undef ARRAY2_SEARCH_SCALAR
#undef ARRAY2_SEARCH_PLAIN_SCALAR
#undef ARRAY_EXTEND
#undef ARRAY_INSERT
#undef ARRAY_REMOVE
#undef ARRAY_FOREACH
#undef ARRAY_SEARCH
#undef ARRAY_SEARCH_SCALAR
#undef ARRAY_SEARCH_PLAIN_SCALAR

/*************************************************************************/

/* Insert `node' into the beginning of `list'.  `node' and `list' must be
 * simple variables (or indirections or array references).
 */
#define LIST_INSERT(node,list)          \
    do {                                \
        node->next = list;              \
        node->prev = NULL;              \
        if (list)                       \
            (list)->prev = node;        \
        list = node;                    \
    } while (0)

/*************************************************************************/

/* Append `node' to the end of `list'.  `node' and `list' must be simple
 * variables (or indirections or array references).
 */
#define LIST_APPEND(node,list)          \
    do {                                \
        typeof(node) *nextptr = &list;  \
        node->next = NULL;              \
        node->prev = NULL;              \
        while (*nextptr) {              \
            node->prev = *nextptr;      \
            nextptr = &((*nextptr)->next);\
        }                               \
        *nextptr = node;                \
    } while (0)

/*************************************************************************/

/* Insert `node' into `list' so that `list' maintains its order as
 * determined by the function `compare' called on `field' of each node.
 * `node' and `list' must be simple variables; `field' must be a field of
 * `node'; and `compare' must be a function that takes two `field's and
 * returns -1, 0, or 1 indicating whether the first argument is ordered
 * before, equal to, or after the second (strcmp, for example).  If an
 * equal node is found, `node' is inserted after it.
 */
#define LIST_INSERT_ORDERED(node,list,compare,field)                      \
    do {                                                                  \
        typeof(node) ptr, prev;                                           \
        for (ptr = list, prev = NULL; ptr; prev = ptr, ptr = ptr->next) { \
            if (compare(node->field, ptr->field) < 0)                     \
                break;                                                    \
        }                                                                 \
        node->next = ptr;                                                 \
        node->prev = prev;                                                \
        if (ptr)                                                          \
            ptr->prev = node;                                             \
        if (prev)                                                         \
            prev->next = node;                                            \
        else                                                              \
            list = node;                                                  \
    } while (0)

/*************************************************************************/

/* Remove `node' from `list'.  `node' and `list' must be simple variables. */
#define LIST_REMOVE(node,list)                  \
    do {                                        \
        if (node->next)                         \
            node->next->prev = node->prev;      \
        if (node->prev)                         \
            node->prev->next = node->next;      \
        else                                    \
            list = node->next;                  \
    } while (0)

/*************************************************************************/

/* Loop over every element in `list', using `iter' as the iterator.  The
 * macro has the same properties as a for() loop.  `iter' must be a simple
 * variable.
 */
#define LIST_FOREACH(iter,list) \
    for (iter = (list); iter; iter = iter->next)

/*************************************************************************/

/* Iterate over `list' using an extra variable (`temp') to hold the next
 * element, ensuring proper operation even when the current element is
 * deleted.
 */
#define LIST_FOREACH_SAFE(iter,list,temp) \
    for (iter = (list); iter && (temp = iter->next, 1); iter = temp)

/*************************************************************************/

/* Search `list' for a node with `field' equal to `target' (as evaluated by
 * `compare') and place a pointer to the node found in `result' (or NULL if
 * none found).  `result' must be a simple variable; `compare' must be a
 * strcmp()-like function (see LIST_INSERT_ORDERED).
 */
#define LIST_SEARCH(list,field,target,compare,result)   \
    do {                                                \
        LIST_FOREACH (result, list) {                   \
            if (compare(result->field, target) == 0)    \
                break;                                  \
        }                                               \
    } while (0)

/*************************************************************************/

/* Search `list' as LIST_SEARCH does, but for a scalar value. */

#define LIST_SEARCH_SCALAR(list,field,target,result)    \
    do {                                                \
        LIST_FOREACH (result, list) {                   \
            if (result->field == target)                \
                break;                                  \
        }                                               \
    } while (0)

/*************************************************************************/

/* Search `list' as LIST_SEARCH does, but for a list known to be ordered. */

#define LIST_SEARCH_ORDERED(list,field,target,compare,result)   \
    do {                                                        \
        LIST_FOREACH (result, list) {                           \
            int i = compare(result->field, target);             \
            if (i > 0)                                          \
                result = NULL;                                  \
            if (i >= 0)                                         \
                break;                                          \
        }                                                       \
    } while (0)

/*************************************************************************/

/* Search `list' as LIST_SEARCH_ORDERED does, but for a scalar value. */

#define LIST_SEARCH_ORDERED_SCALAR(list,field,target,result)    \
    do {                                                        \
        LIST_FOREACH (result, list) {                           \
            int i = result->field - target;                     \
            if (i > 0)                                          \
                result = NULL;                                  \
            if (i >= 0)                                         \
                break;                                          \
        }                                                       \
    } while (0)

/*************************************************************************/

/* Extend a variable-length array by one entry.  `size' is the variable
 * which holds the current length of `array'.
 */
#define ARRAY2_EXTEND(array,size)                       \
    do {                                                \
        (size)++;                                       \
        array = realloc(array, sizeof(*array) * size); \
    } while (0)

/*************************************************************************/

/* Insert a slot at the given position in a variable-length array. */

#define ARRAY2_INSERT(array,size,index)                                 \
    do {                                                                \
        (size)++;                                                       \
        array = realloc(array, sizeof(*array) * size);                 \
        if (index < size-1)                                             \
            memmove(&array[index+1], &array[index],                     \
                    sizeof(*array) * ((size-1)-index));                 \
    } while (0)

/*************************************************************************/

/* Delete entry number `index' from a variable-length array. */

#define ARRAY2_REMOVE(array,size,index)                 \
    do {                                                \
        (size)--;                                       \
        if (index < size)                               \
            memmove(&array[index], &array[index]+1, sizeof(*array)*(size-index));\
        array = realloc(array, sizeof(*array) * size); \
    } while (0)

/*************************************************************************/

/* Loop over every element in a variable-length array. */

#define ARRAY2_FOREACH(iter,array,size) \
    for (iter = 0; iter < size; iter++)

/*************************************************************************/

/* Search a variable-length array for a value.  Operates like LIST_SEARCH.
 * `result' must be an integer variable.  If nothing is found, `result' will
 * be set equal to `size'.
 */
#define ARRAY2_SEARCH(array,size,field,target,compare,result)   \
    do {                                                        \
        ARRAY2_FOREACH (result, array, size) {                  \
            if (compare(array[result].field, target) == 0)      \
                break;                                          \
        }                                                       \
    } while (0)

/*************************************************************************/

/* Search a variable-length array for a value.  The array does not have
 * fields.
 */
#define ARRAY2_SEARCH_PLAIN(array,size,target,compare,result)   \
    do {                                                        \
        ARRAY2_FOREACH (result, array, size) {                  \
            if (compare(array[result], target) == 0)            \
                break;                                          \
        }                                                       \
    } while (0)

/*************************************************************************/

/* Search a variable-length array for a scalar value. */

#define ARRAY2_SEARCH_SCALAR(array,size,field,target,result)    \
    do {                                                        \
        ARRAY2_FOREACH (result, array, size) {                  \
            if (array[result].field == target)                  \
                break;                                          \
        }                                                       \
    } while (0)

/*************************************************************************/

/* Search a variable-length array for a scalar value.  The array does not
 * have fields. */

#define ARRAY2_SEARCH_PLAIN_SCALAR(array,size,target,result)    \
    do {                                                        \
        ARRAY2_FOREACH (result, array, size) {                  \
            if (array[result] == target)                        \
                break;                                          \
        }                                                       \
    } while (0)

/*************************************************************************/

/* Perform the ARRAY2_* actions on an array `array' whose size is stored in
 * `array_count'. */

#define ARRAY_EXTEND(array)       ARRAY2_EXTEND(array,array##_count)
#define ARRAY_INSERT(array,index) ARRAY2_INSERT(array,array##_count,index)
#define ARRAY_REMOVE(array,index) ARRAY2_REMOVE(array,array##_count,index)
#define ARRAY_FOREACH(iter,array) ARRAY2_FOREACH(iter,array,array##_count)
#define ARRAY_SEARCH(array,field,target,compare,result) \
    ARRAY2_SEARCH(array,array##_count,field,target,compare,result)
#define ARRAY_SEARCH_PLAIN(array,target,compare,result) \
    ARRAY2_SEARCH_PLAIN(array,array##_count,target,compare,result)
#define ARRAY_SEARCH_SCALAR(array,field,target,result) \
    ARRAY2_SEARCH_SCALAR(array,array##_count,field,target,result)
#define ARRAY_SEARCH_PLAIN_SCALAR(array,target,result) \
    ARRAY2_SEARCH_PLAIN_SCALAR(array,array##_count,target,result)

/*************************************************************************/

#endif  /* LIST_ARRAY_H */

