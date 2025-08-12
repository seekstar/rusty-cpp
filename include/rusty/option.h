#ifndef RUSTY_OPTION_H_
#define RUSTY_OPTION_H_

#include "rusty/primitive.h"

#include <optional>
#include <type_traits>

namespace rusty {

inline class None {} None;

template <typename T>
class Option;

template <typename T, typename... Args>
Option<T> MakeOption(Args &&...args) {
	return Option<T>(std::in_place, std::forward<Args>(args)...);
}

template <typename T>
class Option {
public:
	Option() : v_(std::nullopt) {}
	Option(class None) : v_(std::nullopt) {}
	Option(T &&x) : v_(std::move(x)) {}

	template <
		typename U,
		typename = std::enable_if_t<std::is_constructible_v<T, U>>
	>
	Option(U &&u) : v_(std::forward<U>(u)) {}

	template <typename... Args>
	Option(
		std::in_place_t, Args &&... args
	) : v_(std::in_place, std::forward<Args>(args)...) {}

	Option<Ref<const T>> as_ref() const {
		if (is_none()) {
			return None;
		}
		return ref(v_.value());
	}
	bool is_none() const {
		return !v_.has_value();
	}
	bool is_some() const {
		return v_.has_value();
	}
	T unwrap_unchecked() && {
		return std::move(v_.value());
	}

	T unwrap() && {
		rusty_assert(is_some());
		return std::move(*this).unwrap_unchecked();
	}

private:
	std::optional<T> v_;
};

template <typename T>
class Option<Ref<T>> : Ref<T> {
public:
	Option() : Ref<T>(nullptr) {}
	Option(class None) : Ref<T>(nullptr) {}
	Option(Ref<T> x) : Ref<T>(x) {}
	Option(std::in_place_t, Ref<T> x) : Ref<T>(x) {}

	Option<Ref<const Ref<T>>> as_ref() const {
		if (is_none()) {
			return None;
		}
		return ref(static_cast<const Ref<T> &>(*this));
	}
	bool is_none() const {
		return this->v_ == nullptr;
	}
	bool is_some() const {
		return this->v_ != nullptr;
	}
	Ref<T> unwrap_unchecked() && {
		return *this;
	}

	Ref<T> unwrap() && {
		rusty_assert(is_some());
		return std::move(*this).unwrap_unchecked();
	}
};
static_assert(sizeof(Option<Ref<int>>) == sizeof(int *));

} // namespace rusty

#endif // RUSTY_OPTION_H_
