#ifndef RUSTY_INTRINSICS_H_
#define RUSTY_INTRINSICS_H_
 
#include <atomic>

namespace rusty {
namespace intrinsics {

// Returns the previous value
template <typename T>
T atomic_max_relaxed(std::atomic<T> &dst, T src) {
	T x = dst.load(std::memory_order_relaxed);
	while (src > x) {
		T expected = x;
		if (dst.compare_exchange_weak(expected, src))
			break;
		x = dst.load(std::memory_order_relaxed);
	}
	return x;
}

} // namespace intrinsics
} // namespace rusty

#endif // RUSTY_INTRINSICS_H_
