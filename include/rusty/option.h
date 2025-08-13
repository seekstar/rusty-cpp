#ifndef RUSTY_OPTION_H_
#define RUSTY_OPTION_H_

#include "rusty/primitive.h"

#include <optional>
#include <type_traits>

namespace rusty {

template <typename T>
class Option;

inline class None {
public:
	template <typename T>
	operator std::optional<T>() {
		return std::nullopt;
	}

	bool operator==(std::nullopt_t) const {
		return true;
	}
} None;

template <typename T, typename... Args>
Option<T> MakeOption(Args &&...args) {
	return Option<T>(std::in_place, std::forward<Args>(args)...);
}

template <typename T>
class Option {
private:
	using value_type = T;

public:
	Option() : v_(std::nullopt) {}
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

	Option(const std::optional<T> &x) : v_(x) {}
	Option(std::optional<T> &&x) : v_(std::move(x)) {}
	operator std::optional<value_type>() && {
		return std::move(v_);
	}

	const T *as_ptr() const {
		if (is_none()) {
			return nullptr;
		}
		return &v_.value();
	}
	T *as_ptr() {
		if (is_none()) {
			return nullptr;
		}
		return &v_.value();
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

	Option(class None) : Option() {}
	Option(std::nullopt_t) : Option() {}

	template <
		typename U = value_type,
		typename = decltype(std::declval<value_type>() == std::declval<U>())
	>
	bool operator==(const Option<U> &rhs) const {
		auto a = as_ptr();
		auto b = rhs.as_ptr();
		if (a == nullptr) {
			return b == nullptr;
		}
		return *a == *b;
	}

	bool operator==(class None) const {
		return is_none();
	}

	value_type unwrap() && {
		rusty_assert(is_some());
		return std::move(*this).unwrap_unchecked();
	}
	Option<value_type> take() {
		Option<value_type> ret;
		std::swap(*this, ret);
		return ret;
	}

private:
	std::optional<T> v_;
};

template <typename T>
class Option<Ref<T>> : Ref<T> {
private:
	using value_type = Ref<T>;

public:
	Option() : Ref<T>(nullptr) {}
	Option(Ref<T> x) : Ref<T>(x) {}
	Option(std::in_place_t, Ref<T> x) : Ref<T>(x) {}

	Option(
		std::optional<Ref<T>> x
	) : Ref<T>(x.has_value() ? x.value() : nullptr) {}

	const Ref<T> *as_ptr() const {
		if (is_none()) {
			return nullptr;
		}
		return this;
	}
	Ref<T> *as_ptr() {
		if (is_none()) {
			return nullptr;
		}
		return this;
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

	operator std::optional<value_type>() && {
		if (is_none()) {
			return std::nullopt;
		}
		return std::move(*this).unwrap_unchecked();
	}

	Option(class None) : Option() {}
	Option(std::nullopt_t) : Option() {}

	template <
		typename U = value_type,
		typename = decltype(std::declval<value_type>() == std::declval<U>())
	>
	bool operator==(const Option<U> &rhs) const {
		auto a = as_ptr();
		auto b = rhs.as_ptr();
		if (a == nullptr) {
			return b == nullptr;
		}
		return *a == *b;
	}

	bool operator==(class None) const {
		return is_none();
	}

	value_type unwrap() && {
		rusty_assert(is_some());
		return std::move(*this).unwrap_unchecked();
	}
	Option<value_type> take() {
		Option<value_type> ret;
		std::swap(*this, ret);
		return ret;
	}
};
static_assert(sizeof(Option<Ref<int>>) == sizeof(int *));

} // namespace rusty

#endif // RUSTY_OPTION_H_
