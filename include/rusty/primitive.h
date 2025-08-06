#ifndef RUSTY_PRIMITIVE_H_
#define RUSTY_PRIMITIVE_H_

#include "rusty/macro.h"

namespace rusty {

namespace detail {

template <typename T>
static inline T lowbit(T x) {
  return x & -x;
}

template <typename T>
bool is_power_of_two_uint(T x) {
	return x == detail::lowbit(x);
}

template <typename T>
T next_power_of_two_uint(T x) {
	T ans = 1;
	while (ans < x) {
		ans <<= 1;
		rusty_assert(ans != 0);
	}
	return ans;
}

} // namespace detail

static inline unsigned char is_power_of_two(unsigned char x) {
	return detail::is_power_of_two_uint(x);
}
static inline unsigned short is_power_of_two(unsigned short x) {
	return detail::is_power_of_two_uint(x);
}
static inline unsigned int is_power_of_two(unsigned int x) {
	return detail::is_power_of_two_uint(x);
}
static inline unsigned long is_power_of_two(unsigned long x) {
	return detail::is_power_of_two_uint(x);
}
static inline unsigned long long is_power_of_two(unsigned long long x) {
	return detail::is_power_of_two_uint(x);
}

// next_power_of_two
// Returns the smallest power of two greater than or equal to "x".
// Panics if overflow.

static inline unsigned char next_power_of_two(unsigned char x) {
	return detail::next_power_of_two_uint(x);
}
static inline unsigned short next_power_of_two(unsigned short x) {
	return detail::next_power_of_two_uint(x);
}
static inline unsigned int next_power_of_two(unsigned int x) {
	return detail::next_power_of_two_uint(x);
}
static inline unsigned long next_power_of_two(unsigned long x) {
	return detail::next_power_of_two_uint(x);
}
static inline unsigned long long next_power_of_two(unsigned long long x) {
	return detail::next_power_of_two_uint(x);
}

}  // namespace rusty

// DEPRECATED
using rusty::next_power_of_two;

#endif // RUSTY_PRIMITIVE_H_
