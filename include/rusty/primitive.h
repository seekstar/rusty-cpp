#ifndef RUSTY_PRIMITIVE_H_
#define RUSTY_PRIMITIVE_H_

#include <rusty/macro.h>

// next_power_of_two
// Returns the smallest power of two greater than or equal to "x".
// Panics if overflow.

template <typename T>
T __next_power_of_two_uint(T x) {
	T ans = 1;
	while (ans < x) {
		ans <<= 1;
		rusty_assert(ans != 0);
	}
	return ans;
}

static inline unsigned char next_power_of_two(unsigned char x) {
	return __next_power_of_two_uint(x);
}
static inline unsigned short next_power_of_two(unsigned short x) {
	return __next_power_of_two_uint(x);
}
static inline unsigned int next_power_of_two(unsigned int x) {
	return __next_power_of_two_uint(x);
}
static inline unsigned long next_power_of_two(unsigned long x) {
	return __next_power_of_two_uint(x);
}
static inline unsigned long long next_power_of_two(unsigned long long x) {
	return __next_power_of_two_uint(x);
}

#endif // RUSTY_PRIMITIVE_H_
