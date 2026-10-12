# Lab 4 diagrams -- Ashwin Anish, AA224496

## pushFront(v)

### Empty list

```text
before:
  head_ ----> NULL
  tail_ ----> NULL
  size_ = 0

  new node n = [v]
  n.prev = NULL
  n.next = NULL

after:
  head_ ----> [v] <---- tail_
              ^
              |
             NULL

  size_ = 1
```

Pointer changes in order:
1. head_ = n
2. tail_ = n
3. size_++

### One element

```text
before:
  head_ ----> [old]
              ^
              |
             NULL
  tail_ ----> [old]

  new node n = [v]

after:
  head_ ----> [v] ----> [old]
              ^          ^
              |          |
             NULL       NULL
  tail_ -------------------> [old]
```

Pointer changes in order:
1. n.next = head_
2. head_.prev = n
3. head_ = n
4. size_++

### Many elements

```text
before:
  head_ ----> [a] <----> [b] <----> [c] <---- tail_

  new node n = [v]

after:
  head_ ----> [v] ----> [a] <----> [b] <----> [c] <---- tail_
              ^
              |
             NULL

  a.prev = n
```

Pointer changes in order:
1. n.next = head_
2. head_.prev = n
3. head_ = n
4. size_++

## pushBack(v)

### Empty list

```text
before:
  head_ ----> NULL
  tail_ ----> NULL
  size_ = 0

  new node n = [v]

after:
  head_ ----> [v] <---- tail_
              ^
              |
             NULL

  size_ = 1
```

Pointer changes in order:
1. head_ = n
2. tail_ = n
3. size_++

### One element

```text
before:
  head_ ----> [old]
  tail_ ----> [old]

  new node n = [v]

after:
  head_ ----> [old] <----> [v]
                      ^
                      |
                     NULL
  tail_ ----------------> [v]
```

Pointer changes in order:
1. n.prev = tail_
2. tail_.next = n
3. tail_ = n
4. size_++

### Many elements

```text
before:
  head_ ----> [a] <----> [b] <----> [c] <---- tail_

  new node n = [v]

after:
  head_ ----> [a] <----> [b] <----> [c] <----> [v]
                                            ^
                                            |
                                           NULL

  c.next = n
```

Pointer changes in order:
1. n.prev = tail_
2. tail_.next = n
3. tail_ = n
4. size_++

## popFront(&out)

### Empty list

```text
before:
  head_ ----> NULL
  tail_ ----> NULL
  size_ = 0

after:
  return false
  *out unchanged
```

### One element

```text
before:
  head_ ----> [x] <---- tail_
              ^
              |
             NULL

  temp = head_
  out = x

after:
  head_ ----> NULL
  tail_ ----> NULL
  size_ = 0

  delete temp
```

Pointer changes in order:
1. temp = head_
2. head_ = head_.next   // NULL
3. if (head_ == NULL) tail_ = NULL
4. head_.prev = NULL if head_ != NULL
5. delete temp
6. size_--

### Many elements

```text
before:
  head_ ----> [a] <----> [b] <----> [c] <---- tail_

  temp = a
  out = a.value

after:
  head_ ----> [b] <----> [c] <---- tail_
              ^
              |
             NULL

  delete a
```

Pointer changes in order:
1. temp = head_
2. head_ = head_.next   // points to b
3. head_.prev = NULL
4. delete temp
5. size_--

## popBack(&out)

### Empty list

```text
before:
  head_ ----> NULL
  tail_ ----> NULL
  size_ = 0

after:
  return false
  *out unchanged
```

### One element

```text
before:
  head_ ----> [x] <---- tail_
              ^
              |
             NULL

  temp = tail_
  out = x

after:
  head_ ----> NULL
  tail_ ----> NULL
  size_ = 0

  delete temp
```

Pointer changes in order:
1. temp = tail_
2. tail_ = tail_.prev   // NULL
3. if (tail_ == NULL) head_ = NULL
4. tail_.next = NULL if tail_ != NULL
5. delete temp
6. size_--

### Many elements

```text
before:
  head_ ----> [a] <----> [b] <----> [c] <---- tail_

  temp = c
  out = c.value

after:
  head_ ----> [a] <----> [b] <---- tail_
                        ^
                        |
                       NULL

  delete c
```

Pointer changes in order:
1. temp = tail_
2. tail_ = tail_.prev   // points to b
3. tail_.next = NULL
4. delete temp
5. size_--

## remove(v)

### HEAD node

```text
before:
  head_ ----> [1] <----> [2] <----> [3] <---- tail_

  current = head_ = 1

after:
  head_ ----> [2] <----> [3] <---- tail_
              ^
              |
             NULL

  2.prev = NULL
```

Pointer changes in order:
1. head_ = current.next
2. current.next.prev = current.prev   // current.prev is NULL, so 2.prev = NULL
3. delete current
4. size_--

### MIDDLE node

```text
before:
  head_ ----> [1] <----> [2] <----> [3] <---- tail_

  current = 2
  current.prev = 1
  current.next = 3

after:
  head_ ----> [1] <----> [3] <---- tail_

  1.next = 3
  3.prev = 1
```

Pointer changes in order:
1. current.prev.next = current.next
2. current.next.prev = current.prev
3. delete current
4. size_--

### TAIL node

```text
before:
  head_ ----> [1] <----> [2] <----> [3] <---- tail_

  current = tail_ = 3

after:
  head_ ----> [1] <----> [2] <---- tail_
                        ^
                        |
                       NULL

  2.next = NULL
```

Pointer changes in order:
1. tail_ = current.prev
2. current.prev.next = current.next   // current.next is NULL, so 2.next = NULL
3. delete current
4. size_--
