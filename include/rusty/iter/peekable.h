#ifndef RUSTY_PEEKABLE_H_
#define RUSTY_PEEKABLE_H_

#include "rusty/iter/iterator.h"

#include <memory>

namespace rusty {

// Trait object for TraitPeekableIterator
template <typename T>
class PeekableIterator : public Iterator<T> {
public:
	static_assert(std::is_same_v<T, typename Iterator<T>::value_type>);
	using value_type = T;

	virtual const value_type *peek(type_tag_t<PeekableIterator<value_type>>) = 0;
	const value_type *peek() {
		return peek(type_tag_t<PeekableIterator<value_type>>());
	}

	template <typename I>
	class FatPointer;
};

template <typename T>
template <typename I>
class PeekableIterator<T>::FatPointer : public PeekableIterator<T> {
public:
	explicit FatPointer(I &&iter) : iter_(std::move(iter)) {}
	Option<T> next(type_tag_t<Iterator<T>>) override {
		return iter_.next(type_tag_t<Iterator<T>>());
	}
	const T* peek(type_tag_t<PeekableIterator<T>>) override {
		return iter_.peek(type_tag_t<PeekableIterator<T>>());
	}

private:
	I iter_;
};

// impl TraitPeekableIterator
template <typename I>
class Peekable {
public:
	using value_type = typename I::value_type;

	explicit Peekable(I &&iter) : iter_(std::move(iter)) {}

	Option<value_type> next(type_tag_t<Iterator<value_type>>) {
		if (peeked_.is_some()) {
			return peeked_.take();
		}
		return iter_.next(type_tag_t<Iterator<value_type>>());
	}
	Option<value_type> next() {
		return next(type_tag_t<Iterator<value_type>>());
	}

	const value_type *peek(type_tag_t<PeekableIterator<value_type>>) {
		auto peeked = peeked_.as_ptr();
		if (peeked != nullptr) {
			return peeked;
		}
		peeked_ = iter_.next(type_tag_t<Iterator<value_type>>());
		return peeked_.as_ptr();
	}
	const value_type *peek() {
		return peek(type_tag_t<PeekableIterator<value_type>>());
	}

private:
	I iter_;
	Option<value_type> peeked_;
};

template <typename I, typename = std::enable_if_t<!detail::IteratorImpl<I>::impl>>
Peekable<I> MakePeekable(I &&iter) {
	return Peekable<I>(std::forward<I>(iter));
}

template <typename I, typename = std::enable_if_t<detail::IteratorImpl<I>::impl>>
Peekable<detail::IteratorImpl<I>> MakePeekable(I &&iter) {
  return MakePeekable(detail::IteratorImpl<I>(std::forward<I>(iter)));
}

template <typename I>
auto MakeTraitObject(
	Peekable<I> &&peekable
) -> std::unique_ptr<PeekableIterator<typename I::value_type>> {
	return std::make_unique<
		typename PeekableIterator<typename I::value_type>
			::template FatPointer<Peekable<I>>
	>(std::move(peekable));
}

} // namespace rusty

#endif // RUSTY_PEEKABLE_H_
