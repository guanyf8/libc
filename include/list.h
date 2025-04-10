#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include <stddef.h>
#include <basic.h>

struct list_node{
	struct list_node* next;
};

#define LIST_HEAD_INIT(name) { &(name)}

#define LIST_HEAD(name) \
	struct list_node name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_node *list)
{
	list->next = list;
}

/*
 * Insert a _new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_node *_new,
			      struct list_node *prev,
			      struct list_node *next)
{
	_new->next = next;
	prev->next = _new;
}

/**
 * list_add - add a _new entry
 * @_new: _new entry to be added
 * @head: list head to add it after
 *
 * Insert a _new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_node *_new, struct list_node *head)
{
	__list_add(_new, head, head->next);
}

/**
 * list_add_tail - add a _new entry
 * @_new: _new entry to be added
 * @head: list head to add it before
 *
 * Insert a _new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_node *_new, struct list_node *prev)
{
	__list_add(_new, prev, prev->next);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_node * prev, struct list_node * next)
{
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void list_del(struct list_node *entry,struct list_node* prev)
{
	__list_del(prev, entry->next);
	entry->next = NULL;
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_node *entry,struct list_node* prev)
{
	__list_del(prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(struct list_node *list, struct list_node* prev,
								struct list_node *head)
{
        __list_del(prev, list->next);
        list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(struct list_node *list,  struct list_node* prev,
				  struct list_node *head)
{
        __list_del(prev, list->next);
        list_add_tail(list, head);
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const struct list_node *head)
{
	return head->next == head;
}

static inline void __list_splice(struct list_node *list, struct list_node* last,
				 struct list_node *head)
{
	struct list_node *first = list->next;
	struct list_node *at = head->next;

	head->next = first;
	last->next = at;
}

/**
 * list_splice - join two lists
 * @list: the _new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(struct list_node *list, struct list_node* last,
							   struct list_node *head)
{
	if (!list_empty(list))
		__list_splice(list, last, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the _new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(struct list_node *list,  struct list_node* last,
				    struct list_node *head)
{
	if (!list_empty(list)) {
		__list_splice(list, last, head);
		INIT_LIST_HEAD(list);
	}
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_node pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_node to use as a loop counter.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; prefetch(pos->next), pos != (head); \
        	pos = pos->next)

/**
 * __list_for_each	-	iterate over a list
 * @pos:	the &struct list_node to use as a loop counter.
 * @head:	the head for your list.
 *
 * This variant differs from list_for_each() in that it's the
 * simplest possible list iteration code, no prefetching is done.
 * Use this for code that knows the list to be very short (empty
 * or 1 entry) most of the time.
 */
#define __list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_safe	-	iterate over a list safe against removal of list entry
 * @pos:	the &struct list_node to use as a loop counter.
 * @n:		another &struct list_node to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     prefetch(pos->member.next), &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_prepare_entry - prepare a pos entry for use as a start point in
 *			list_for_each_entry_continue
 * @pos:	the type * to use as a start point
 * @head:	the head of the list
 * @member:	the name of the list_struct within the struct.
 */
#define list_prepare_entry(pos, head, member) \
	((pos) ? : list_entry(head, typeof(*pos), member))

/**
 * list_for_each_entry_continue -	iterate over list of given type
 *			continuing after existing point
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_continue(pos, head, member) 		\
	for (pos = list_entry(pos->member.next, typeof(*pos), member);	\
	     prefetch(pos->member.next), &pos->member != (head);	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

/**
 * list_for_each_entry_safe_continue -	iterate over list of given type
 *			continuing after existing point safe against removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe_continue(pos, n, head, member) 		\
	for (pos = list_entry(pos->member.next, typeof(*pos), member), 		\
		n = list_entry(pos->member.next, typeof(*pos), member);		\
	     &pos->member != (head);						\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))


/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct dlist_node {
	struct dlist_node *next, *prev;
};

#define DLIST_HEAD_INIT(name) { &(name), &(name) }

#define DLIST_HEAD(name) \
	struct dlist_node name = DLIST_HEAD_INIT(name)

static inline void INIT_DLIST_HEAD(struct dlist_node *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a _new entry between two known consecutive entries.
 *
 * This is only for internal dlist manipulation where we know
 * the prev/next entries already!
 */
static inline void __dlist_add(struct dlist_node *_new,
			      struct dlist_node *prev,
			      struct dlist_node *next)
{
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}

/**
 * dlist_add - add a _new entry
 * @_new: _new entry to be added
 * @head: dlist head to add it after
 *
 * Insert a _new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void dlist_add(struct dlist_node *_new, struct dlist_node *head)
{
	__dlist_add(_new, head, head->next);
}

/**
 * dlist_add_tail - add a _new entry
 * @_new: _new entry to be added
 * @head: dlist head to add it before
 *
 * Insert a _new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void dlist_add_tail(struct dlist_node *_new, struct dlist_node *head)
{
	__dlist_add(_new, head->prev, head);
}

/*
 * Delete a dlist entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal dlist manipulation where we know
 * the prev/next entries already!
 */
static inline void __dlist_del(struct dlist_node * prev, struct dlist_node * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * dlist_del - deletes entry from dlist.
 * @entry: the element to delete from the dlist.
 * Note: dlist_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void dlist_del(struct dlist_node *entry)
{
	__dlist_del(entry->prev, entry->next);
	entry->prev=NULL;
	entry->next=NULL;
}

/**
 * dlist_del_init - deletes entry from dlist and reinitialize it.
 * @entry: the element to delete from the dlist.
 */
static inline void dlist_del_init(struct dlist_node *entry)
{
	__dlist_del(entry->prev, entry->next);
	INIT_DLIST_HEAD(entry);
}

/**
 * dlist_move - delete from one dlist and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void dlist_move(struct dlist_node *list, struct dlist_node *head)
{
        __dlist_del(list->prev, list->next);
        dlist_add(list, head);
}

/**
 * dlist_move_tail - delete from one dlist and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void dlist_move_tail(struct dlist_node *list,
				  struct dlist_node *head)
{
        __dlist_del(list->prev, list->next);
        dlist_add_tail(list, head);
}

/**
 * dlist_empty - tests whether a dlist is empty
 * @head: the dlist to test.
 */
static inline int dlist_empty(const struct dlist_node *head)
{
	return head->next == head;
}

/**
 * dlist_empty_careful - tests whether a dlist is
 * empty _and_ checks that no other CPU might be
 * in the process of still modifying either member
 *
 * NOTE: using dlist_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the dlist entry is dlist_del_init(). Eg. it cannot be used
 * if another CPU could re-dlist_add() it.
 *
 * @head: the dlist to test.
 */
static inline int dlist_empty_careful(const struct dlist_node *head)
{
	struct dlist_node *next = head->next;
	return (next == head) && (next == head->prev);
}

static inline void __dlist_splice(struct dlist_node *list,
				 struct dlist_node *head)
{
	struct dlist_node *first = list->next;
	struct dlist_node *last = list->prev;
	struct dlist_node *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/**
 * dlist_splice - join two dlists
 * @list: the _new dlist to add.
 * @head: the place to add it in the first dlist.
 */
static inline void dlist_splice(struct dlist_node *list, struct dlist_node *head)
{
	if (!dlist_empty(list))
		__dlist_splice(list, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the _new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void dlist_splice_init(struct dlist_node *list,
				    struct dlist_node *head)
{
	if (!dlist_empty(list)) {
		__dlist_splice(list, head);
		INIT_DLIST_HEAD(list);
	}
}

/**
 * dlist_entry - get the struct for this entry
 * @ptr:	the &struct dlist_node pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the dlist_struct within the struct.
 */
#define dlist_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * dlist_for_each	-	iterate over a dlist
 * @pos:	the &struct dlist_node to use as a loop counter.
 * @head:	the head for your list.
 */
#define dlist_for_each(pos, head) \
	for (pos = (head)->next; prefetch(pos->next), pos != (head); \
        	pos = pos->next)

/**
 * __dlist_for_each	-	iterate over a list
 * @pos:	the &struct dlist_node to use as a loop counter.
 * @head:	the head for your list.
 *
 * This variant differs from dlist_for_each() in that it's the
 * simplest possible dlist iteration code, no prefetching is done.
 * Use this for code that knows the dlist to be very short (empty
 * or 1 entry) most of the time.
 */
#define __dlist_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * dlist_for_each_prev	-	iterate over a dlist backwards
 * @pos:	the &struct dlist_node to use as a loop counter.
 * @head:	the head for your dlist.
 */
#define dlist_for_each_prev(pos, head) \
	for (pos = (head)->prev; prefetch(pos->prev), pos != (head); \
        	pos = pos->prev)

/**
 * dlist_for_each_safe	-	iterate over a dlist safe against removal of dlist entry
 * @pos:	the &struct dlist_node to use as a loop counter.
 * @n:		another &struct dlist_node to use as temporary storage
 * @head:	the head for your dlist.
 */
#define dlist_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * dlist_for_each_entry	-	iterate over dlist of given type
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your dlist.
 * @member:	the name of the dlist_struct within the struct.
 */
#define dlist_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     prefetch(pos->member.next), &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * dlist_for_each_entry_reverse - iterate backwards over dlist of given type.
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your dlist.
 * @member:	the name of the dlist_struct within the struct.
 */
#define dlist_for_each_entry_reverse(pos, head, member)			\
	for (pos = list_entry((head)->prev, typeof(*pos), member);	\
	     prefetch(pos->member.prev), &pos->member != (head); 	\
	     pos = list_entry(pos->member.prev, typeof(*pos), member))

/**
 * dlist_prepare_entry - prepare a pos entry for use as a start point in
 *			dlist_for_each_entry_continue
 * @pos:	the type * to use as a start point
 * @head:	the head of the dlist
 * @member:	the name of the dlist_struct within the struct.
 */
#define dlist_prepare_entry(pos, head, member) \
	((pos) ? : list_entry(head, typeof(*pos), member))

/**
 * dlist_for_each_entry_continue -	iterate over list of given type
 *			continuing after existing point
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define dlist_for_each_entry_continue(pos, head, member) 		\
	for (pos = list_entry(pos->member.next, typeof(*pos), member);	\
	     prefetch(pos->member.next), &pos->member != (head);	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * dlist_for_each_entry_safe - iterate over dlist of given type safe against removal of dlist entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your dlist.
 * @member:	the name of the dlist_struct within the struct.
 */
#define dlist_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

/**
 * dlist_for_each_entry_safe_continue -	iterate over dlist of given type
 *			continuing after existing point safe against removal of dlist entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your dlist.
 * @member:	the name of the dlist_struct within the struct.
 */
#define dlist_for_each_entry_safe_continue(pos, n, head, member) 		\
	for (pos = list_entry(pos->member.next, typeof(*pos), member), 		\
		n = list_entry(pos->member.next, typeof(*pos), member);		\
	     &pos->member != (head);						\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

/**
 * dlist_for_each_entry_safe_reverse - iterate backwards over dlist of given type safe against
 *				      removal of dlist entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your dlist.
 * @member:	the name of the dlist_struct within the struct.
 */
#define dlist_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = list_entry((head)->prev, typeof(*pos), member),	\
		n = list_entry(pos->member.prev, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.prev, typeof(*n), member))

/*
 * Double linked lists with a single pointer list head.
 * Mostly useful for hash tables where the two pointer list head is
 * too wasteful.
 * You lose the ability to access the tail in O(1).
 */

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};

#define HLIST_HEAD_INIT { .first = NULL }
#define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
static inline void INIT_HLIST_NODE(struct hlist_node *h)
{
	h->next = NULL;
	h->pprev = NULL;
}

static inline int hlist_unhashed(const struct hlist_node *h)
{
	return !h->pprev;
}

static inline int hlist_empty(const struct hlist_head *h)
{
	return !h->first;
}

static inline void __hlist_del(struct hlist_node *n)
{
	struct hlist_node *next = n->next;
	struct hlist_node **pprev = n->pprev;
	*pprev = next;
	if (next)
		next->pprev = pprev;
}

static inline void hlist_del(struct hlist_node *n)
{
	__hlist_del(n);
	n->next = NULL;
	n->pprev = NULL;
}

static inline void hlist_del_init(struct hlist_node *n)
{
	if (n->pprev)  {
		__hlist_del(n);
		INIT_HLIST_NODE(n);
	}
}

static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
	struct hlist_node *first = h->first;
	n->next = first;
	if (first)
		first->pprev = &n->next;
	h->first = n;
	n->pprev = &h->first;
}

/* next must be != NULL */
static inline void hlist_add_before(struct hlist_node *n,
					struct hlist_node *next)
{
	n->pprev = next->pprev;
	n->next = next;
	next->pprev = &n->next;
	*(n->pprev) = n;
}

static inline void hlist_add_after(struct hlist_node *n,
					struct hlist_node *next)
{
	next->next = n->next;
	n->next = next;
	next->pprev = &n->next;

	if(next->next)
		next->next->pprev  = &next->next;
}

#define hlist_entry(ptr, type, member) container_of(ptr,type,member)

#define hlist_for_each(pos, head) \
	for (pos = (head)->first; pos && ({ prefetch(pos->next); 1; }); \
	     pos = pos->next)

#define hlist_for_each_safe(pos, n, head) \
	for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
	     pos = n)

/**
 * hlist_for_each_entry	- iterate over list of given type
 * @tpos:	the type * to use as a loop counter.
 * @pos:	the &struct hlist_node to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry(tpos, pos, head, member)			 \
	for (pos = (head)->first;					 \
	     pos && ({ prefetch(pos->next); 1;}) &&			 \
		({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = pos->next)

/**
 * hlist_for_each_entry_continue - iterate over a hlist continuing after existing point
 * @tpos:	the type * to use as a loop counter.
 * @pos:	the &struct hlist_node to use as a loop counter.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_continue(tpos, pos, member)		 \
	for (pos = (pos)->next;						 \
	     pos && ({ prefetch(pos->next); 1;}) &&			 \
		({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = pos->next)

/**
 * hlist_for_each_entry_from - iterate over a hlist continuing from existing point
 * @tpos:	the type * to use as a loop counter.
 * @pos:	the &struct hlist_node to use as a loop counter.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_from(tpos, pos, member)			 \
	for (; pos && ({ prefetch(pos->next); 1;}) &&			 \
		({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = pos->next)

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @tpos:	the type * to use as a loop counter.
 * @pos:	the &struct hlist_node to use as a loop counter.
 * @n:		another &struct hlist_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_safe(tpos, pos, n, head, member) 		 \
	for (pos = (head)->first;					 \
	     pos && ({ n = pos->next; 1; }) && 				 \
		({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = n)


#endif
