### Simple standard library
Is a single header file library for : better string handling, and also comes with a lot of math related functions.

## Modularity
You can remove whatever you don't want with a simple #define :
- SSTDLIB_NO_MATH : remove all math functions
- SSTDLIB_NO_STRING : remove all string related functions
- SSTDLIB_NO_INT : remove all integer related functions
- SSTDLIB_NO_DOUBLE : (same)
- SSTDLIB_NO_FLOAT : (same)

## Notes
You do not need to include neither of stdlib.h and stdio.h, as they are already included within it.

Also, it is a prototype, which is why there is not a lot of features in it.
Although, it does its job.
