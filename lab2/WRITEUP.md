# ECE 412 -- Lab 2 Writeup

**Name:**
**EID:**
**Machine used (hostname):**

*Answer in your own words -- two to four sentences each unless noted. These are the explanation points of the rubric; code alone does not earn them.*

---

## W1. Why is `struct DB` in `db.cpp` and not `db.h`?

State what a client of `db.h` can and cannot do with a `DB` when the struct definition is hidden. Give one concrete misuse (a line of code) that the hidden layout physically prevents.

A client of db.h can only see the typedef DB * it cannot declare a DB and cannot access any of the db elements. That is the whole point of the info hiding, we dont want the user to use the db without using the helper functions for security and functional reasons. 

## W2. Deep copy of names

`DB_add(db, 42, "Ada", 3.9)` must NOT store the caller's `"Ada"` pointer -- it must `my_strdup` its own copy. In one or two sentences, describe the dangling-pointer bug you would create if you stored the caller's pointer directly. Which valgrind category would that show up as?

The original pointer could be free or resued, which could create a dangling poitner. That would produce a user-after-free bug and valgrind would report it as a "invalid read/write" style issue. 

## W3. Why does `DB_find` return `const Record *`?

Explain what a client could do with a non-const `Record *` that would break the ADT's invariants. Give two concrete misuses (`free(r->name)` is one -- what is another?).

If it returned a mutable Record, a client could modify its contents such as r->id and r->gpa. They could also free r->name which would free memory still owned by the DB and corrupt the Database. Making it constant means the user now cant make changes to things we want to keep safe. 

## W4. Removal strategy

Which did you pick for `DB_remove`: **shift the tail down** or **swap the last element into the gap**? Why? State the big-O of your choice and the one visible side effect (order preserved vs. not).

Then walk through, by hand, removing the last element of a 3-element DB using your strategy. Off-by-one at the last element is the specific bug this exercise catches.

I picked the swapt the last element into the gap strategy. It is O(1) and is simpler than shifiting the tail. If we had [A,B,C] and we remove B, then I move C into the slot 1. The last elelment is moved forward. The off by one bug is forgetting that the last valid index is count-1 so you dont read past teh allocated memory locations. 

## W5. `DB_destroy` order

You free `items[i].name` for each `i`, then `db->items`, then `db`. What goes wrong if you do it in the OPPOSITE order (free `db` first, or free `db->items` before the names)? Which valgrind category catches each?

If you fre the db first you lose access to the array and the names. If db->items is cleared first then every name pointer becomes oprphaned nand valgrind reports it as indirect leaks. The valgrind categories are indirectly lost or definitely lost.

## W6. Growth policy

State your starting capacity, your growth factor, and the capacity sequence your >= 20-record driver actually produced. Confirm that `sizeof(Record)` (not `sizeof(int)`) is what you passed to `malloc`/`realloc` -- one sentence on why hard-coding a number here would break the moment we added a field to `Record`.

Starting capacity is 4. The growth facter is cap*2. The sequence was 4,8,16,32. The array is resized with newcap * sizeof(Record), not with a hard-coded number, because sizeof(Record) automatically reflects the actual size of the struct. If I hard-coded a number, the code would be wrong as soon as a field was added or changed in Record.

## W7. Save/load round trip

Show one line from your `records.txt`, one line from your `bad_records.txt`, and describe how `DB_load` decides the second one is malformed. What does your `DB_load` do to the DB when it encounters the bad line -- roll back, keep partial, something else? Whatever you chose, say so plainly.

records.txt 17,rrrrr,2.30

bad records.txt 101,Yvonne,not_a_number

The load function decides the second line is malormed because the gpa feild is not a valid floating point number. the fscanf cannot match it correcclty. In my implementation, when the malformed line is encountered, the function stops, closes the file, and returns 0, keeping the DB in whatever valid state it had before the bad record.



## W8. Separate compilation

Run:

```
touch main.cpp
make
```

Paste the `make` output. Only `main.o` and the final link should rebuild -- `db.o` and `mystr.o` must NOT recompile. If they do, your Makefile is doing "recompile everything on any change" and you lose the separate-compilation points.

g++ -std=c++11 -Wall -Wextra -Werror -g -c main.cpp -o main.o
g++ -std=c++11 -Wall -Wextra -Werror -g mystr.o db.o main.o -o db

## W9. The valgrind summary (paste)

Paste the `HEAP SUMMARY` through `ERROR SUMMARY` block of

`valgrind --leak-check=full --show-leak-kinds=all ./db`

It must show 0 errors, 0 bytes definitely lost, 0 bytes indirectly lost.

```
==...== HEAP SUMMARY:
==...==     in use at exit: 0 bytes in 1 blocks
==...==   total heap usage: ... allocs, ... frees, ... bytes allocated
==...==
==...== All heap blocks were freed -- no leaks are possible
==...==
==...== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## W10. AI assistance

Name **one specific bug you had to fix in AI-suggested code** on this lab -- or state plainly that you wrote the lab unaided. AI-assisted lines in your source carry the syllabus citation comment.

Two common ones on this lab, if you need a starting point: (a) the assistant put `struct DB { ... }` in `db.h`, killing the 15 opacity points; (b) `DB_remove` shifted the array down without `free(items[i].name)` first, leaking every removed name.

no ai assistance

## W11. Anything that fought you *(optional, genuinely useful)*
