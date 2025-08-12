#ifndef RUSTY_RESULT_H_
#define RUSTY_RESULT_H_

#include "rusty/macro.h"

#include <variant>

namespace rusty {

template <typename T, typename E>
class [[nodiscard]] Result : std::variant<T, E> {
public:
	Result(T &&v) : std::variant<T, E>(std::move(v)) {}
	Result(E &&e) : std::variant<T, E>(std::move(e)) {}

	template <
		typename U,
		typename = std::enable_if_t<std::is_constructible_v<T, U>>
	>
	Result(U &&u) : std::variant<T, E>(std::forward<U>(u)) {}

	bool is_ok() const { return this->index() == 0; }
	bool is_err() const { return this->index() == 1; }
	T unwrap_unchecked() && {
		return std::move(std::get<0>(*this));
	}
	T unwrap() && {
		rusty_assert(is_ok());
		return std::move(*this).unwrap_unchecked();
	}
	E unwrap_err_unchecked() && {
		return std::move(std::get<1>(*this));
	}
	E unwrap_err() && {
		rusty_assert(is_err());
		return std::move(*this).unwrap_err_unchecked();
	}
};

} // namespace rusty

#define rusty_check_result_impl(name, expr) ({ \
	auto && (name) = (expr); \
	if ((name).is_err()) { \
		return std::move(name).unwrap_err_unchecked(); \
	} \
	std::move(name).unwrap_unchecked(); \
})

#define rusty_check_result(expr) \
	rusty_check_result_impl(rusty_unique_name(), expr)

#endif // RUSTY_RESULT_H_
