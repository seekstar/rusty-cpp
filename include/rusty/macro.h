#ifndef RUSTY_MACRO_H_
#define RUSTY_MACRO_H_

#include <cstdio>
#include <cstdlib>

#define rusty_panic(...) do { \
    fprintf(stderr, "panic: %s:%u: %s:", \
        __FILE__, __LINE__, __func__); \
    fprintf(stderr, " " __VA_ARGS__);   \
    abort(); \
} while (0)

#define rusty_assert(cond, ...) do { \
    if (!(cond)) { rusty_panic(__VA_ARGS__); }   \
} while (0)

#endif // RUSTY_MACRO_H_
