#ifndef RUSTY_ITERATOR_H_
#define RUSTY_ITERATOR_H_

#include "rusty/option.h"

#include <functional>
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
};

template <typename I>
void collect_into(I iter, std::vector<typename I::value_type> &v) {
	for (;;) {
		auto res = iter.next(type_tag_t<Iterator<typename I::value_type>>());
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

} // namespace rusty

#endif // RUSTY_ITERATOR_H_
