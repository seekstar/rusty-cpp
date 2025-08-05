#ifndef RUSTY_PEEKABLE_H_
#define RUSTY_PEEKABLE_H_

#include "rusty/macro.h"

#include <cassert>
#include <functional>
#include <optional>

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

	std::optional<PeekMut> peek_mut() {
		rusty_assert(!mut_borrowed_);
		if (v_.empty()) {
			return std::nullopt;
		}
		return PeekMut(*this);
	}

	std::optional<T> pop() {
		rusty_assert(!mut_borrowed_);
		if (v_.empty()) {
			return std::nullopt;
		}
		std::optional<T> ret = std::move(v_[0]);
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

}  // namespace rusty

#endif // RUSTY_PEEKABLE_H_
