#ifndef RUSTY_MACRO_H_
#define RUSTY_MACRO_H_

#include <cstdio>
#include <cstdlib>

#define rusty_panic(...) do { \
	/* The "if" should be optimized out by the compiler */ \
	if (sizeof(#__VA_ARGS__) == 1) { \
		fprintf(stderr, "panic: %s:%u: %s\n", \
			__FILE__, __LINE__, __func__); \
	} else { \
		fprintf(stderr, "panic: %s:%u: %s", \
			__FILE__, __LINE__, __func__); \
		fprintf(stderr, ": " __VA_ARGS__);   \
		fprintf(stderr, "\n"); \
	} \
	abort(); \
} while (0)

#define rusty_assert(cond, ...) do { \
	if (!(cond)) { rusty_panic("Assertion `" #cond "` failed"); }   \
} while (0)

#endif // RUSTY_MACRO_H_
