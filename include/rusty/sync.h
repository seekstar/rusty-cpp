#ifndef RUSTY_SYNC_H_
#define RUSTY_SYNC_H_

#include <mutex>

namespace rusty {
namespace sync {

template <typename T>
class Mutex;

template <typename T>
class MutexGuard {
public:
	T &operator*() const { return data_.get(); }
	T *operator->() const { return &data_.get(); }
private:
	MutexGuard(
		T &data, std::unique_lock<std::mutex> &&lock
	) : data_(data), lock_(std::move(lock)) {}
	std::reference_wrapper<T> data_;
	std::unique_lock<std::mutex> lock_;
	friend class Mutex<T>;
};

template <typename T>
class Mutex {
public:
	Mutex(T &&data) : data_(std::move(data)) {}
	MutexGuard<T> lock() const {
		return MutexGuard<T>(data_, std::unique_lock(lock_));
	}
private:
	mutable T data_;
	mutable std::mutex lock_;
};

} // namespace sync
} // namespace rusty

#endif // RUSTY_SYNC_H_
