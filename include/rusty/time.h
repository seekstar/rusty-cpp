#ifndef RUSTY_TIME_H_
#define RUSTY_TIME_H_

#include <chrono>

namespace rusty {
namespace time {
class Duration {
public:
	Duration(const Duration &duration) : Duration(duration.nsec_) {}
	Duration &operator=(const Duration &rhs) {
		nsec_ = rhs.nsec_;
		return *this;
	}
	static Duration from_nanos(uint64_t nsec) { return Duration(nsec); }
	uint64_t as_nanos() const {
		return nsec_;
	}
	double as_secs_double() const {
		return nsec_ / 1e9;
	}
	Duration operator+(Duration rhs) const {
		return Duration(nsec_ + rhs.nsec_);
	}
private:
	Duration(uint64_t nsec) : nsec_(nsec) {}
	// uint128_t is not in standard C++ yet.
	uint64_t nsec_;
};
class Instant {
public:
	static Instant now() {
		return std::chrono::steady_clock::now();
	}
	Duration elapsed() const {
		return now() - *this;
	}
	Duration operator-(const Instant& earlier) {
		return Duration::from_nanos(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				time_ - earlier.time_).count());
	}
private:
	Instant(std::chrono::steady_clock::time_point time) : time_(time) {}
	std::chrono::steady_clock::time_point time_;
};
} // namespace time
} // namespace rusty

#endif // RUSTY_TIME_H_
