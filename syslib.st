// --- SPITELANG SYSTEM LIBRARY (syslib.st) ---
// Global Library for lists, strings, and arithmetic

@sys_init
0 ⊞ // Initialize Void[0] as the heap allocation pointer
↩

@malloc
// Pop Alpha (size), return Pointer in Alpha
0 ⊟ ø // Get current heap head
» » » // Move P away from 0
↩

// --- STRING UTILS ---

@print_str
// Loop until Void[P] == 0 is reached
@str_loop
⊟ ø ¿
§ » ^str_loop
?
↩

@str_new
// Expects characters on Alpha stack, terminated by 0
@str_store_loop
ø ¿
⊞ » ^str_store_loop
?
↩

// --- MATH UTILS ---

@add
// Adds Alpha top to Beta top, result in Alpha
@add_loop
∇ ¿
∆ ^add_loop
?
↩