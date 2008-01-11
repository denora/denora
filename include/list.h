/* List Abstract Data Type
 *
 * © 2004-2008 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id$
 *
 */
/*
 * List Abstract Data Type
 * Copyright (C) 1997 Kaz Kylheku <kaz@ashi.footprints.net>
 *
 * Free Software License:
 *
 * All rights are reserved by the author, with the following exceptions:
 * Permission is granted to freely reproduce and distribute this software,
 * possibly in exchange for a fee, provided that this copyright notice appears
 * intact. Permission is also granted to adapt this software to produce
 * derivative works, as long as the modified versions carry this copyright
 * notice and additional notices stating that the work has been modified.
 * This source code may be translated into executable form and incorporated
 * into proprietary software; there is no requirement for such software to
 * contain a copyright notice related to this source.
 *
 */
/*************************************************************************/

#ifndef LIST_H
#define LIST_H

#define LISTCOUNT_T_MAX ULONG_MAX

	typedef unsigned long listcount_t;

	typedef struct lnode_t {
		struct lnode_t *list_next;
		struct lnode_t *list_prev;
		void *list_data;
	} lnode_t;

	typedef struct lnodepool_t {
		struct lnode_t *list_pool;
		struct lnode_t *list_free;
		listcount_t list_size;
	} lnodepool_t;

	typedef struct list_t {
		lnode_t list_nilnode;
		listcount_t list_nodecount;
		listcount_t list_maxcount;
	} list_t;

E lnode_t *lnode_create (void *);
E lnode_t *lnode_init (lnode_t *, void *);
E void lnode_destroy (lnode_t *);
E void lnode_put (lnode_t *, void *);
E void *lnode_get (lnode_t *);
E int lnode_is_in_a_list (lnode_t *);

#define lnode_put(N, D)		((N)->list_data = (D))
#define lnode_get(N)		((N)->list_data)

E lnodepool_t *lnode_pool_init (lnodepool_t *, lnode_t *, listcount_t);
E lnodepool_t *lnode_pool_create (listcount_t);
E void lnode_pool_destroy (lnodepool_t *);
E lnode_t *lnode_borrow (lnodepool_t *, void *);
E void lnode_return (lnodepool_t *, lnode_t *);
E int lnode_pool_isempty (lnodepool_t *);
E int lnode_pool_isfrom (lnodepool_t *, lnode_t *);
E int comparef (const void *, const void *);
E list_t *list_init (list_t *, listcount_t);
E list_t *list_create (listcount_t);
E void list_destroy (list_t *);
E void list_destroy_nodes (list_t *);
E void list_return_nodes (list_t *, lnodepool_t *);
E listcount_t list_count (list_t *);
E int list_isempty (list_t *);
E int list_isfull (list_t *);
E int list_contains (list_t *, lnode_t *);
E void list_append (list_t *, lnode_t *);
E void list_prepend (list_t *, lnode_t *);
E void list_ins_before (list_t *, lnode_t *, lnode_t *);
E void list_ins_after (list_t *, lnode_t *, lnode_t *);

E lnode_t *list_first (list_t *);
E lnode_t *list_last (list_t *);
E lnode_t *list_next (list_t *, lnode_t *);
E lnode_t *list_prev (list_t *, lnode_t *);

E lnode_t *list_del_first (list_t *);
E lnode_t *list_del_last (list_t *);
E lnode_t *ListDelete (list_t *, lnode_t *);

E void list_process (list_t *, void *, void (*)(list_t *, lnode_t *, void *));

E int list_verify (list_t *);

#define lnode_pool_isempty(P)	((P)->list_free == 0)
#define list_count(L)		((L)->list_nodecount)
#define list_isempty(L)		((L)->list_nodecount == 0)
#define list_isfull(L)		(L->list_nodecount == (L)->list_maxcount)
#define list_next(L, N)		(N->list_next == &(L)->list_nilnode ? NULL : (N)->list_next)
#define list_prev(L, N)		(N->list_prev == &(L)->list_nilnode ? NULL : (N)->list_prev)
#define list_last(L)		list_prev(L, &(L)->list_nilnode)

#define list_append(L, N)	list_ins_before(L, N, &(L)->list_nilnode)
#define list_prepend(L, N)	list_ins_after(L, N, &(L)->list_nilnode)
#define list_del_first(L)	ListDelete(L, list_first(L))
#define list_del_last(L)	ListDelete(L, list_last(L))

/* destination list on the left, source on the right */

E void list_extract (list_t *, list_t *, lnode_t *, lnode_t *);
E void list_transfer (list_t *, list_t *, lnode_t * first);
E void list_merge (list_t *, list_t *, int (const void *, const void *));
E void list_sort (list_t *, int (const void *, const void *));
E lnode_t *list_find (list_t *, const void *, int (const void *, const void *));
E int list_is_sorted (list_t *, int (const void *, const void *));

#endif

