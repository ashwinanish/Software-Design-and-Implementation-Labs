/* ------------------------------------------------------------------ *
 * deque.cpp -- ECE 412 Lab 4.  Your implementation of deque.h.
 *
 * Every function is stubbed.  Draw the box-and-arrow diagrams in
 * DIAGRAMS.md first -- empty, one element, many elements -- and write
 * each function from its diagram.  The diagram is the work; the code
 * is the transcription.
 *
 * Invariants to keep true before and after every public function:
 *   I1. head_ == NULL  <=>  tail_ == NULL  <=>  size_ == 0
 *   I2. head_->prev == NULL  and  tail_->next == NULL
 *   I3. for every node n with a next:  n->next->prev == n
 *   I4. size_ is the number of nodes reachable from head_
 *
 * Allowed: new/delete, std::cout.  Not allowed: malloc/free,
 * references, operators you define yourself, standard-library
 * containers.  See the table in README.md.
 * ------------------------------------------------------------------ */
#include <iostream>

#include "deque.h"

/* ---- lifetime ---------------------------------------------------- */

Deque::Deque()
{
    /* TODO: set head_, tail_ and size_ in a member-initializer list */
    head_ = NULL;
    tail_ = NULL;
    size_ = 0;
}

Deque::~Deque()
{
    /* TODO: delete every node */
    clear();
}

void Deque::clear()
{
    /* TODO: walk the list deleting each node */ 
    Node *current = head_;
    while (current != NULL) {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head_ = NULL;
    tail_ = NULL;
    size_ = 0;
}

/* ---- the two ends ------------------------------------------------ */

void Deque::pushFront(int v)
{
    /* TODO: new Node(v) gives a node whose prev and next are NULL. */
    Node *newNode = new Node(v);
    if (head_ == NULL) {
        head_ = newNode;
        tail_ = newNode;
    } 
    else {
        newNode->next = head_;
        head_->prev = newNode;
        head_ = newNode;
    }
    size_++;
}

void Deque::pushBack(int v)
{
    /* TODO: the mirror image of pushFront, through tail_ */ 
    Node *newNode = new Node(v);
    if (tail_ == NULL) {
        head_ = newNode;
        tail_ = newNode;
    }
    else {
        newNode->prev = tail_;
        tail_->next = newNode;
        tail_ = newNode;
    }
    size_++;
}

bool Deque::popFront(int *out)
{
    /* TODO: false (and *out untouched) if empty.  Otherwise write the
     * value to *out and return true */ 
    if (head_ == NULL) {
        return false;
    }
    *out = head_->value;
    Node *temp = head_;
    head_ = head_->next;
    if (head_ != NULL) {
        head_->prev = NULL;
    } 
    else {
        tail_ = NULL;
    }
    delete temp;
    size_--;
    return true;
}

bool Deque::popBack(int *out)
{
    /* TODO: the mirror image of popFront, through tail_ and prev. */
    if (tail_ == NULL) {
        return false;
    }
    *out = tail_->value;
    Node *temp = tail_;
    tail_ = tail_->prev;
    if (tail_ != NULL) {
        tail_->next = NULL;
    } 
    else {
        head_ = NULL;
    }
    delete temp;
    size_--;
    return true;
}

bool Deque::front(int *out) const
{
    /* TODO: peek.  false if empty; nothing is removed. */
    if (head_ == NULL) {
        return false;
    }
    *out = head_->value;
    return true;
}

bool Deque::back(int *out) const
{
    /* TODO */
    if (tail_ == NULL) {
        return false;
    }
    *out = tail_->value;
    return true;
}

/* ---- search and removal ------------------------------------------ */

bool Deque::contains(int v) const
{
    /* TODO */ 
    Node *current = head_;
    while (current != NULL) {
        if (current->value == v) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool Deque::remove(int v)
{
    /* TODO: walk to the FIRST node holding v; false if there is none.
     * Then unlink it */ 
    if(!contains(v)) {
        return false;
    }
    Node *current = head_;
    while (current != NULL) {
        if (current->value == v) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } 
            else {
                head_ = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            } 
            else {
                tail_ = current->prev;
            }
            delete current;
            size_--;
            return true;
        }
        current = current->next;
    }
    return false;
}

/* ---- observers --------------------------------------------------- */

std::size_t Deque::size() const
{
    return size_;
}

bool Deque::empty() const
{
    if (size_ == 0) {
        return true;
    }
    return false;
}

void Deque::printForward() const
{
    /* TODO: one line, then '\n':
     *     [ ]                    empty
     *     [ 1 <-> 2 <-> 3 ]      head to tail  */
    std::cout << "[ ";
    Node *current = head_;
    while (current != NULL) {
        std::cout << current->value;
        if (current->next != NULL) {
            std::cout << " <-> ";
        }
        current = current->next;
    }
    std::cout << " ]" << std::endl;
}

void Deque::printBackward() const
{
    /* TODO: the same format, tail to head, following prev */ 
    std::cout << "[ ";
    Node *current = tail_;
    while (current != NULL) {
        std::cout << current->value;
        if (current->prev != NULL) {
            std::cout << " <-> ";
        }
        current = current->prev;
    }
    std::cout << " ]" << std::endl;
}
