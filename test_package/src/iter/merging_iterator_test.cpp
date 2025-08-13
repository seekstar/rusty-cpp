#include "rusty/iter/iterator.h"
#include "rusty/iter/merging_iterator.h"
#include "rusty/iter/peekable.h"
#include "test.h"

#include <gtest/gtest.h>

namespace {
void check_equal(
	const std::vector<rusty::Ref<const int>> &a, const std::vector<int> &b
) {
	ASSERT_EQ(a.size(), b.size());
	for (size_t i = 0; i < a.size(); ++i) {
		ASSERT_EQ(a[i].deref(), b[i]);
	}
}
}

TEST_F(Test, MergingIteratorSimple) {
	std::vector<int> a{0, 2, 4, 6, 8};
	std::vector<int> b{1, 3, 5, 7, 9};
	std::vector<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	{
		std::vector<
			std::unique_ptr<rusty::PeekableIterator<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeIter(a.data(), a.data()))
		));
		iters.push_back(rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeIter(b.data(), b.data()))
		));
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_TRUE(v.empty());
	}
	{
		std::vector<
			std::unique_ptr<rusty::PeekableIterator<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeIter(a))
		));
		iters.push_back(rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeIter(b.data(), b.data()))
		));
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_NO_FATAL_FAILURE(check_equal(v, a));
	}
	{
		std::vector<
			std::unique_ptr<rusty::PeekableIterator<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeIter(a.data(), a.data()))
		));
		iters.push_back(rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeIter(b))
		));
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_NO_FATAL_FAILURE(check_equal(v, b));
	}
	{
		std::vector<
			std::unique_ptr<rusty::PeekableIterator<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeIter(a))
		));
		iters.push_back(rusty::MakeTraitObject(
			rusty::MakePeekable(rusty::MakeIter(b))
		));
		std::vector<rusty::Ref<const int>> v;
		auto iter = rusty::NewMergingIterator(std::move(iters));
		rusty::collect_into(std::move(iter), v);
		ASSERT_NO_FATAL_FAILURE(check_equal(v, c));
	}
}
