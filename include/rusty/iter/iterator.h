#ifndef RUSTY_ITERATOR_H_
#define RUSTY_ITERATOR_H_

#include "rusty/option.h"

#include <functional>
#include <memory>
#include <vector>

namespace rusty {

template <typename T>
struct type_tag_t {};

// Trait object for TraitIterator
template <typename T>
class Iterator {
public:
	using value_type = T;
	virtual ~Iterator() = default;
	virtual Option<T> next(type_tag_t<Iterator<value_type>>) = 0;

	Option<value_type> next() {
		return next(type_tag_t<Iterator<value_type>>());
	}

	template <typename I>
	class FatPointer;
};

template <typename T>
template <typename I>
class Iterator<T>::FatPointer : public Iterator<T> {
public:
	explicit FatPointer(I &&iter) : iter_(std::move(iter)) {}
	Option<T> next(type_tag_t<Iterator<T>>) override {
		return iter_.next(type_tag_t<Iterator<T>>());
	}

private:
	I iter_;
};

namespace detail {

template <typename T>
class IteratorImpl : public T {
public:
	template <typename... Args>
	IteratorImpl(Args &&... args) : T(std::forward<Args>(args)...) {}
};

template <typename Iter>
class IteratorImpl<std::unique_ptr<Iter>> {
public:
	using value_type = typename Iter::value_type;
	explicit IteratorImpl(
		std::unique_ptr<Iter> iter
	) : iter_(std::move(iter)) {}
	Option<value_type> next(type_tag_t<Iterator<value_type>>) {
		return iter_->next();
	}

private:
	std::unique_ptr<Iter> iter_;
};

} // namespace detail

template <typename I>
void collect_into(
	I iter,
	std::vector<typename detail::IteratorImpl<I>::value_type> &v
) {
	auto it = detail::IteratorImpl<I>(std::move(iter));
	for (;;) {
		auto res = it.next(
			type_tag_t<Iterator<typename detail::IteratorImpl<I>::value_type>>()
		);
		if (res.is_none()) {
			break;
		}
		v.push_back(std::move(res).unwrap_unchecked());
	}
}

template <typename T>
class Iter {
public:
	using value_type = Ref<const T>;
	Iter(const T *start, const T *end) : it_(start), end_(end) {}
	Option<value_type> next(type_tag_t<Iterator<value_type>>) {
		if (it_ == end_) {
			return None;
		}
		auto ret = ref(*it_);
		++it_;
		return ret;
	}

	Option<value_type> next() {
		return next(type_tag_t<Iterator<value_type>>());
	}
private:
	const T *it_;
	const T *end_;
};

template <typename T>
Iter<T> MakeIter(const T *start, const T *end) {
	return Iter<T>(start, end);
}

template <typename T>
std::unique_ptr<Iterator<Ref<const T>>> MakeTraitObject(Iter<T> iter) {
	return std::make_unique<
		typename Iterator<Ref<const T>>::template FatPointer<Iter<T>>
	>(std::move(iter));
}

} // namespace rusty

#endif // RUSTY_ITERATOR_H_
