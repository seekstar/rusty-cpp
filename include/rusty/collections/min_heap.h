#ifndef RUSTY_PEEKABLE_H_
#define RUSTY_PEEKABLE_H_

#include <functional>
#include <optional>

namespace rusty {

template <typename T, typename Compare = std::less<T>>
class MinHeap {
public:
	explicit MinHeap(
		std::vector<T> v = {}, Compare compare = Compare()
	) : v_(std::move(v)), cmp_(std::move(compare)) {
		for (ssize_t i = v_.size() / 2 - 1; i >= 0; --i) {
			heapify_subtree(i);
		}
	}

	bool is_empty() const {
		return v_.empty();
	}

	const T *peek() const {
		if (v_.empty()) {
			return nullptr;
		}
		return &v_[0];
	}

	/**
	 * Do nothing if the heap is empty.
	 * @param f: return true if the root needs to be removed from the heap
	 */
	void update_root(std::function<bool(T&)> f) {
		if (v_.empty()) {
			return;
		}
		if (f(v_[0])) {
			remove_root();
		} else {
			heapify_subtree(0);
		}
	}

	std::optional<T> pop() {
		std::optional<T> ret;
		update_root([&ret](T &x) {
			ret = std::move(x);
			return true;
		});
		return ret;
	}

private:
	void heapify_subtree(size_t i) {
		size_t smallest = i;
		size_t left = (i << 1) + 1;
		if (left < v_.size() && cmp_(v_[left], v_[smallest])) {
			smallest = left;
		}
		size_t right = (i << 1) + 2;
		if (right < v_.size() && cmp_(v_[right], v_[smallest])) {
			smallest = right;
		}
		if (smallest == i) {
			return;
		}
		std::swap(v_[i], v_[smallest]);
		heapify_subtree(smallest);
	}

	void remove_root() {
		if (v_.size() > 1) {
			v_[0] = std::move(v_.back());
		}
		v_.pop_back();
		heapify_subtree(0);
	}

	std::vector<T> v_;
	Compare cmp_;
};

}  // namespace rusty

#endif // RUSTY_PEEKABLE_H_
