#ifndef RUSTY_RESULT_H_
#define RUSTY_RESULT_H_

#include <rusty/macro.h>
#include <variant>

namespace rusty {

template <typename T, typename E>
class Result : public std::variant<T, E> {
public:
	Result(T &&v) : std::variant<T, E>(std::move(v)) {}
	Result(E &&e) : std::variant<T, E>(std::move(e)) {}
	Result(const E &e) : std::variant<T, E>(e) {}

	bool is_ok() const { return this->index() == 0; }
	bool is_err() const { return this->index() == 1; }
	T &&unwrap() {
		rusty_assert(is_ok());
		return std::move(std::get<0>(*this));
	}
	E &&unwrap_err() {
		rusty_assert(is_err());
		return std::move(std::get<1>(*this));
	}
};

} // namespace rusty

#define rusty_check(arg) ({											\
	auto &&result = std::forward<std::decay_t<decltype(arg)>>(arg);	\
	if (result.index() == 1)										\
		return std::move(std::get<1>(std::move(result)));			\
	std::move(std::get<0>(std::move(result)));						\
})

#endif // RUSTY_RESULT_H_
