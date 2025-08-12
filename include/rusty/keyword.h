#ifndef RUSTY_KEYWORD_H_
#define RUSTY_KEYWORD_H_

#include <variant>

namespace rusty {

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

/// Usage:
/// match(value, [](Type1 type1) {
/// 		;
/// 	},
/// 	[](Type2 type2) {
/// 		;
/// 	},
/// 	...
/// 	[](TypeN typeN) {
/// 		;
/// 	}
/// );
template <typename Val, typename... Ts>
auto match(Val &&val, Ts... ts) {
	return std::visit(overloaded{ts...}, std::forward<Val>(val));
}

} // namespace rusty

#endif // RUSTY_KEYWORD_H_
