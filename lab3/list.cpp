/* ------------------------------------------------------------------ *
 * list.cpp -- ECE 412 Lab 3.  The implementation, and the ONLY place
 *             `struct Node` and `struct List` are allowed to exist.
 *
 * C subset of C++11 only.  malloc/free, char *, no classes, no STL.
 * Every function below has a box-and-arrow diagram you should draw
 * before you write it.  The diagram is the work; the code is the
 * transcription.
 * ------------------------------------------------------------------ */
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "list.h"
#include "mystr.h"

/* ------------------------------------------------------------------ *
 * The private shape.  These two definitions never leave this file.
 * ------------------------------------------------------------------ */

struct Node {
    Item  item;
    Node *next;
};

struct List {
    Node   *head;
    Node   *tail;   /* required: List_insert_back must be constant time */
    size_t  count;  /* required: List_size must be constant time       */
};

/* ------------------------------------------------------------------ *
 * Recommended private helpers.  Uncomment and fill these in.
 *
 * node_new is a single place that builds one fully formed node --
 * name deep-copied, next set to NULL -- and returns NULL if either
 * malloc fails, having freed whatever it already took.  Writing it
 * first makes both inserts about three lines long, and satisfies
 * "a failed allocation must not corrupt the list" in one place
 * instead of two.
 *
 *   static Node *node_new(int sku, const char *name, int qty)
 *   {
 *       ...
 *   }
 *
 *   static void node_free(Node *n)    // frees the name AND the node
 *   {
 *       ...
 *   }
 * ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ *
 * lifetime
 * ------------------------------------------------------------------ */

static Node *node_new(int sku, const char *name, int qty)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        return NULL;
    }
    n->item.sku = sku;
    n->item.name = my_strdup(name);
    if (n->item.name == NULL) {
        free(n);
        return NULL;
    }
    n->item.qty = qty;
    n->next = NULL;
    return n;
}

static void node_free(Node *n)
{
    if (n != NULL) {
        free(n->item.name);
        free(n);
    }
}

List *List_create(void)
{
    List *l = (List *)malloc(sizeof(List));
    if (l == NULL) {
        return NULL;
    }
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
    return l;
}

void List_destroy(List *l)
{
    /* TODO: List_destroy(NULL) is a no-op.  Otherwise free everything in the 
     * correct order.*/
    if (l == NULL) {
        return;
    }
    Node *current = l->head;
    while (current != NULL) {
        Node *next = current->next;
        node_free(current);
        current = next;
    }
    free(l);
}

/* ------------------------------------------------------------------ *
 * insertion
 * ------------------------------------------------------------------ */

int List_insert_front(List *l, int sku, const char *name, int qty)
{
    /* TODO: allocate, deep-copy the name, link at the head. */
    Node *n = node_new(sku, name, qty);
    if (n == NULL) {
        return 0;
    }
    n->next = l->head;
    l->head = n;
    if (l->tail == NULL) {
        l->tail = n;
    }
    l->count++;
    return 1;
}

int List_insert_back(List *l, int sku, const char *name, int qty)
{
    /* TODO: constant time via l->tail.  Do NOT walk the list.
     * On an empty list, head and tail both become the new node. */
    Node *n = node_new(sku, name, qty);
    if (n == NULL) {
        return 0;
    }
    n->next = NULL;
    if (l->head == NULL) {
        l->head = n;
    } 
    else {
        l->tail->next = n;
    }
    l->tail = n;
    l->count++;
    return 1;
}

/* ------------------------------------------------------------------ *
 * lookup
 * ------------------------------------------------------------------ */

const Item *List_find(const List *l, int sku)
{
    /* TODO: return  requested item or NULL.
     * Note the const: this function may not modify anything. */
    Node *current = l->head;
    while (current != NULL) {
        if (current->item.sku == sku) {
            return &(current->item);
        }
        current = current->next;
    }
    return NULL;
}

/* ------------------------------------------------------------------ *
 * removal
 * ------------------------------------------------------------------ */

int List_remove(List *l, int sku)
{
    /* TODO: four cases, and you must test all four:
     *   1. empty list                  
     *   2. match is the head           
     *   3. match is in the middle      
     *   4. match is the tail          
     */
    if (l->head == NULL) {
        return 0; 
    }
    if (l->head->item.sku == sku) {
        Node *nexts = l->head->next;
        node_free(l->head);
        l->head = nexts;
        l->count--;
        if (l->head == NULL) {
            l->tail = NULL;
        }
        return 1;
    }
    else if(l->tail->item.sku == sku) {
        Node *current = l->head;
        while (current->next != l->tail) {
            current = current->next;
        }
        node_free(l->tail);
        l->tail = current;
        l->tail->next = NULL;
        l->count--;
        return 1;
    }
    else {
        Node *current = l->head;
        while (current->next != NULL) {
            if (current->next->item.sku == sku) {
                Node *temp = current->next;
                current->next = temp->next;
                node_free(temp);
                l->count--;
                return 1;
            }
            current = current->next;
        }
    }
    return 0;
}

int List_remove_front(List *l, Item *out)
{
    /* TODO: implement, not forgetting to check if it is already empty */
    if (l == NULL) {
        return 0;
    }
    if (l->head == NULL) {
        return 0; 
    }
    Node *temp = l->head;
    l->head = l->head->next;
    if (l->head == NULL) {
        l->tail = NULL;
    }
    if (out != NULL) {
    *out = temp->item;
    }
    free(temp);
    l->count--;
    return 1;
}

/* ------------------------------------------------------------------ *
 * observers
 * ------------------------------------------------------------------ */

size_t List_size(const List *l)
{
    /* TODO: return the maintained count.*/
    if (l == NULL) {
        return 0;
    }
    return l->count;
}

int List_empty(const List *l)
{
    if (l == NULL) {
        return 1;
    }
    if (l->count == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void List_clear(List *l)
{
    /* TODO: same freeing loop as List_destroy, but the List survives
     * and must be left genuinely reusable. */ 
    Node *current = l->head;
    while (current != NULL) {
        Node *next = current->next;
        node_free(current);
        current = next;
    }
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
}

void List_print(const List *l)
{
    /* TODO: exactly one line, exactly this shape:
     *     [ ]
     *     [ (101,"bolt",4) -> (102,"nut",9) ]
    * printf only.  No std::cout -- it is banned this week. */
    if (l == NULL) {
        printf("[ ]\n");
        return;
    }
    printf("[ ");
    Node *current = l->head;
    while (current != NULL) {
        printf("(%d,\"%s\",%d)", current->item.sku, current->item.name, current->item.qty);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" ]\n");
}

void List_for_each(const List *l, void (*fn)(const Item *it, void *ctx),
                   void *ctx)
{
    /* TODO: call fn once per item, front to back, passing ctx
     * through untouched.  NULL fn is a no-op.  No Node * escapes --
     * this is how you get iteration out of an opaque type in C. */
    if (l==NULL || fn == NULL) {
        return;
    }
    Node *current = l->head;
    while (current != NULL) {
        fn(&(current->item), ctx);
        current = current->next;
    }
}