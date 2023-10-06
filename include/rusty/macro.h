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
	if (!(cond)) { \
		rusty_panic("Assertion `" #cond "` failed. " __VA_ARGS__); \
	} \
} while (0)

#define __rusty_first_arg(a, ...) a
#define __rusty_drop_first_arg_with_prepended_comma(a, ...) , ## __VA_ARGS__

#define rusty_assert_eq(l, r, ...) do { \
	auto left= (l); \
	auto right= (r); \
	if (sizeof(#__VA_ARGS__) == 1) { \
		rusty_assert( \
			left == right, "left = %s, right = %s", \
			std::to_string(left).c_str(), std::to_string(right).c_str() \
		); \
	} else { \
		rusty_assert( \
			left == right, "left = %s, right = %s. " __rusty_first_arg(__VA_ARGS__), \
			std::to_string(left).c_str(), std::to_string(right).c_str() \
			__rusty_drop_first_arg_with_prepended_comma(__VA_ARGS__) \
		); \
	} \
} while (0)

#endif // RUSTY_MACRO_H_
