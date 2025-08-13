#include "rusty/iter/iterator.h"
#include "rusty/iter/peekable.h"
#include "test.h"

#include <gtest/gtest.h>

namespace {
template <typename I>
void check(I &&iter, const std::vector<size_t> &a) {
	for (size_t i = 0; i < a.size(); ++i) {
		ASSERT_TRUE(iter.peek() != nullptr);
		ASSERT_EQ(iter.peek()->deref(), i);
		ASSERT_EQ(iter.next().unwrap().deref(), i);
	}
	ASSERT_TRUE(iter.peek() == nullptr);
	ASSERT_TRUE(iter.next().is_none());
}
} // namespace

TEST_F(Test, PeekableSimple) {
	std::vector<size_t> a;
	for (size_t i = 0; i < 10; ++i) {
		a.push_back(i);
	}
	auto iter = rusty::MakePeekable(rusty::MakeIter(a.data(), a.data()));
	ASSERT_TRUE(iter.peek() == nullptr);
	ASSERT_TRUE(iter.next().is_none());

	ASSERT_NO_FATAL_FAILURE(check(rusty::MakePeekable(rusty::MakeIter(a)), a));
	ASSERT_NO_FATAL_FAILURE(check(
		rusty::MakePeekable(rusty::MakeTraitObject(rusty::MakeIter(a))),
		a
	));
	ASSERT_NO_FATAL_FAILURE(check(
		*rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeTraitObject(rusty::MakeIter(a)))
		),
		a
	));
}
