# ECE 412 — Lab 1 Writeup

**Name: Ashwin Anish**
**EID: AA224496**
**Machine used (hostname): Mario**

*Answer in your own words — two to four sentences each unless noted. These are the explanation points of the rubric; code alone does not earn them.*

---

## W1. The growth policy

State your growth policy (starting capacity, growth factor, when growth triggers), and give the capacity sequence your 40-push driver actually produced.

The Starting capacity is 4, the growth factor is double the current capacity, and the growth trigger is whenever the sizes equals the cap. The push sequence is 4, 8, 16, 32, 64. 

## W2. `realloc` vs. the manual version

You implemented growth both ways. What did `realloc` do that your malloc+copy+free version did not (or vice versa)? Include the **addresses you observed**: paste two or three `data=` lines from your driver output for each version and say whether the buffer moved at each doubling.

The realloc grows the existing buffer in one call and can move it to a new larger block if needed. The manual version does the same but allocated a new block, copies the old elements, and then frees the old block with a new pointer and capacity. 

push  0: ... data=0x5b62c80
push  1: ... data=0x5b63110

## W3. The `realloc` trap

`a->data = (int *)realloc(a->data, n);` is a memory leak whenever `realloc` fails. Explain exactly what is lost, and why assigning to a temporary first fixes it. (Your `ia_grow` must also *do* it correctly — this question is the "say why.")

This code overwrittes before checking whether the reallocation succeeded. If it fails the old heap block is still vailed but the program lost the only poitner to it so it leaks. If we do it withthe temporary pointer the old buffer is not lost. 

## W4. The stale pointer after growth

Why is holding `int *p = &a->data[0];` across an `ia_push_back` a use-after-free? Two or three sentences. 

The pointer becomes free after the pushback grows the array becaue the old buffer may be freed and replaced with a larger one. After that p points to the freed memory. 

## W5. Two-level ownership

Your driver builds an array of `my_strdup`'d strings. Describe the two levels of ownership and the order you freed them in. What does valgrind report if you free the array of pointers but not the strings — which leak category?

The driver builds a two level ownership structure with the names (an array of char pointers) and names[i] the strings. each have to be seperately allocated and freed. If I freed only the array of pointers and not the strings, Valgrind would report those string allocations as definitely lost, because the strings would be unreachable heap blocks.

## W6. The `+1`, and the cast

Why does `my_strdup` allocate `my_strlen(s) + 1` bytes — what goes wrong with exactly `my_strlen(s)`, and which tool catches it? And note the C/C++ difference on `malloc`'s return value (quiz material).

It does the plus one because the terminal \0 byte needs to be allocated. If i allocated exactly the lengths bytes, there would be no terminator leading the buffer to be malformed.Malloc returns void in Cpp so it has to be casted to the right type. 

## W7. Unsigned comparison

Why must `my_strcmp` compare as `unsigned char`? Give the concrete example from the handout (`"\xFF"` vs `"a"`) and say what plain signed `char` gets wrong on the ECE Linux server machines. 

On the ECE Linux machines, char is signed. This means that the value 0xFF is -1. When we compare this against 'a' it marks it as lower. Making it signed makes us compare the raw value. 

## W8. The valgrind summary (paste)

Paste the full summary block of

`valgrind --leak-check=full --show-leak-kinds=all ./toolkit`

— the lines from `HEAP SUMMARY` through `ERROR SUMMARY`. It must show 0 errors, 0 bytes definitely lost, 0 bytes indirectly lost.

HEAP SUMMARY:
==1048250==     in use at exit: 0 bytes in 0 blocks
==1048250==   total heap usage: 50 allocs, 50 frees, 74,468 bytes allocated

==1048250== All heap blocks were freed -- no leaks are possible

==1048250== For lists of detected and suppressed errors, rerun with: -s
==1048250== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

## W9. AI assistance

Name **one specific bug you had to fix in AI-suggested code** on this lab (what the assistant wrote, why it was wrong, how you fixed it) — or state plainly that you wrote the lab unaided. AI-assisted lines in your source carry the syllabus citation comment.

I wrote the lab unaided.

## W10. Anything that fought you *(optional, genuinely useful)*
