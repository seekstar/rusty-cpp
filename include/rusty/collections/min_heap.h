#ifndef RUSTY_MIN_HEAP_H_
#define RUSTY_MIN_HEAP_H_

#include "rusty/macro.h"
#include "rusty/option.h"

#include <cassert>
#include <vector>

namespace rusty {

template <typename T, typename Compare = std::less<T>>
class MinHeap {
public:
	class PeekMut {
	public:
		PeekMut(const PeekMut &) = delete;
		PeekMut &operator=(const PeekMut &) = delete;
		PeekMut(PeekMut &&peeked) : heap_(peeked.heap_) {
			peeked.heap_ = nullptr;
		}
		PeekMut &operator=(PeekMut &&peeked) {
			~PeekMut();
			heap_ = peeked.heap_;
			peeked.heap_ = nullptr;
			return *this;
		}

		~PeekMut() {
			if (heap_ == nullptr) {
				return;
			}
			heap_->heapify_subtree(0);
			rusty_assert(heap_->mut_borrowed_);
			heap_->mut_borrowed_ = false;
		}

		T &operator*() {
			return heap_->v_[0];
		}
		T *operator->() {
			return &heap_->v_[0];
		}
		T pop() && {
			T ret = std::move(heap_->v_[0]);
			heap_->remove_root();
			rusty_assert(heap_->mut_borrowed_);
			heap_->mut_borrowed_ = false;
			heap_ = nullptr;
			return ret;
		}

	private:
		PeekMut(MinHeap<T, Compare> &heap) : heap_(&heap) {
			heap_->mut_borrowed_ = true;
		}
		MinHeap<T, Compare> *heap_;
		friend class MinHeap;
	};

	explicit MinHeap(
		std::vector<T> v = {}, Compare compare = Compare()
	) : v_(std::move(v)), cmp_(std::move(compare)) {
		for (ssize_t i = v_.size() / 2 - 1; i >= 0; --i) {
			heapify_subtree(i);
		}
	}
	~MinHeap() {
		rusty_assert(!mut_borrowed_);
	}

	MinHeap(MinHeap &&rhs) : v_(std::move(rhs.v_)), cmp_(std::move(rhs.cmp_)) {
		rusty_assert(!mut_borrowed_);
	}
	MinHeap &operator=(MinHeap &&rhs) {
		rusty_assert(!mut_borrowed_);
		v_ = std::move(rhs.v_);
		cmp_ = std::move(rhs.cmp_);
		return *this;
	}

	bool is_empty() const {
		rusty_assert(!mut_borrowed_);
		return v_.empty();
	}

	const T *peek() const {
		rusty_assert(!mut_borrowed_);
		if (v_.empty()) {
			return nullptr;
		}
		return &v_[0];
	}

	Option<PeekMut> peek_mut() {
		rusty_assert(!mut_borrowed_);
		if (v_.empty()) {
			return None;
		}
		return PeekMut(*this);
	}

	Option<T> pop() {
		rusty_assert(!mut_borrowed_);
		if (v_.empty()) {
			return None;
		}
		Option<T> ret = std::move(v_[0]);
		remove_root();
		return ret;
	}

private:
	void heapify_subtree(size_t i) {
		assert(i < v_.size());
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
	void heapify_root_if_exists() {
		if (v_.empty()) {
			return;
		}
		heapify_subtree(0);
	}

	void remove_root() {
		if (v_.size() > 1) {
			v_[0] = std::move(v_.back());
		}
		v_.pop_back();
		heapify_root_if_exists();
	}

	std::vector<T> v_;
	Compare cmp_;
	bool mut_borrowed_ = false;
};

template <typename T, typename Compare = std::less<T>>
MinHeap<T, Compare> MakeMinHeap(std::vector<T> v, Compare compare = Compare()) {
	return MinHeap<T, Compare>(std::move(v), std::move(compare));
}

}  // namespace rusty

#endif // RUSTY_MIN_HEAP_H_
