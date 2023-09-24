#ifndef RUSTY_MEM_H_
#define RUSTY_MEM_H_

#include <utility>

namespace rusty {
namespace mem {

template <typename T>
class ManuallyDrop {
public:
	ManuallyDrop() {}
	ManuallyDrop(const ManuallyDrop<T> &rhs) = delete;
	ManuallyDrop &operator=(const ManuallyDrop<T> &rhs) = delete;
	ManuallyDrop(ManuallyDrop<T> &&rhs) : v_(std::move(rhs.v_)) {}
	ManuallyDrop<T> &operator=(ManuallyDrop<T> &&rhs) {
		v_ = std::move(rhs.v_);
		return *this;
	}
	~ManuallyDrop() {}
	ManuallyDrop &operator=(T &&v) { v_ = std::move(v); return *this; }
	T into_inner() { return std::move(v_); }
	void drop() { v_.~T(); }
private:
	union {
		T v_;
	};
};

} // namespace mem
} // namespace rusty

#endif // RUSTY_MEM_H_
