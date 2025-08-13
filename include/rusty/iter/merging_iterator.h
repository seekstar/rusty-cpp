#ifndef RUSTY_MERGING_ITERATOR_H_
#define RUSTY_MERGING_ITERATOR_H_

#include "rusty/collections/min_heap.h"
#include "rusty/iter/iterator.h"
#include "rusty/iter/peekable.h"

namespace rusty {

template <typename T, typename Compare = std::less<T>>
class MergingIterator : public Iterator<T> {
public:
	// PeekMut references heap_, so this class can't copy or move.
	MergingIterator(MergingIterator<T, Compare> &&) = delete;
	MergingIterator &operator=(MergingIterator<T, Compare> &&rhs) = delete;

	explicit MergingIterator(
		std::vector<std::unique_ptr<PeekableIterator<T>>> iters,
		Compare cmp = Compare()
	) : heap_(
			(eraseEmptyIters(iters), std::move(iters)), IterCmp(std::move(cmp))
		)
	{}

	Option<T> next(type_tag_t<Iterator<T>>) override {
		auto maybe_top = heap_top_.take();
		if (maybe_top.is_some()) {
			auto top = std::move(maybe_top).unwrap_unchecked();
			if ((*top)->peek() == nullptr) {
				std::move(top).pop();
			}
		}
		heap_top_ = heap_.peek_mut();
		auto top_ptr = heap_top_.as_ptr();
		if (top_ptr == nullptr) {
			return None;
		}
		auto &it = **top_ptr;
		return it->next();
	}

private:
	using I = std::unique_ptr<PeekableIterator<T>>;

	class IterCmp {
	public:
		explicit IterCmp(Compare cmp) : cmp_(std::move(cmp)) {}
		bool operator()(const I &a, const I &b) {
			const T *ax = a->peek();
			assert(ax != nullptr);
			const T *bx = b->peek();
			assert(bx != nullptr);
			return cmp_(*ax, *bx);
		}

	private:
		Compare cmp_;
	};

	static void eraseEmptyIters(
		std::vector<std::unique_ptr<PeekableIterator<T>>>& iters
	) {
		// Use erase_if after upgrading to C++20
		size_t i = 0;
		for (size_t j = 0; j < iters.size(); ++j) {
			if (iters[j]->peek() != nullptr) {
				if (i != j) {
					iters[i] = std::move(iters[j]);
					iters[j] = nullptr;
				}
				++i;
			}
		}
		iters.resize(i);
	}

	MinHeap<I, IterCmp> heap_;
	// It is a common practice in C++ for iterators to keep the returned value
	// alive until the next call to "next" or "peek". Therefore, we keep PeekMut
	// here to freeze the heap until the next call to "next", so that the heap
	// won't peek the underlying iterator and invalidate the returned value
	// in the meantime.
	Option<typename MinHeap<I, IterCmp>::PeekMut> heap_top_;
};

template <typename T, typename Compare = std::less<T>>
std::unique_ptr<Iterator<T>> NewMergingIterator(
	std::vector<std::unique_ptr<PeekableIterator<T>>> iters,
	Compare cmp = Compare()
) {
	return std::make_unique<MergingIterator<T, Compare>>(
		std::move(iters), std::move(cmp)
	);
}

} // namespace rusty

#endif // RUSTY_MERGING_ITERATOR_H_
