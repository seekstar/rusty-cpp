#ifndef RUSTY_MACRO_H_
#define RUSTY_MACRO_H_

#include <cstdio>
#include <cstdlib>
#include <string>

#define rusty_concat(a, b) a ## b
#define rusty_unique_name_impl(counter) rusty_concat(_name_, counter)
#define rusty_unique_name() rusty_unique_name_impl(__COUNTER__)

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

#define rusty_assert_eq_impl(lname, l, rname, r, fmt, ...) do { \
	auto lname= (l); \
	auto rname= (r); \
	rusty_assert( \
		lname == rname, "left = %s, right = %s. " fmt, \
		std::to_string(lname).c_str(), std::to_string(rname).c_str() \
		, ## __VA_ARGS__ \
	); \
} while (0)

#define rusty_assert_eq(l, r, ...) \
	rusty_assert_eq_impl( \
		rusty_unique_name(), l, rusty_unique_name(), r, "" __VA_ARGS__ \
	)

#define rusty_assert_ne_impl(lname, l, rname, r, fmt, ...) do { \
	auto lname= (l); \
	auto rname= (r); \
	rusty_assert( \
		lname != rname, "left = %s, right = %s. " fmt, \
		std::to_string(lname).c_str(), std::to_string(rname).c_str() \
		, ## __VA_ARGS__ \
	); \
} while (0)

#define rusty_assert_ne(l, r, ...) \
	rusty_assert_ne_impl( \
		rusty_unique_name(), l, rusty_unique_name(), r, "" __VA_ARGS__ \
	)

#endif // RUSTY_MACRO_H_
