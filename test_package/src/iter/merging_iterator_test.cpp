#include "rusty/iter/merging_iterator.h"
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

class Iterator {
public:
	Iterator(
		int start, int end, int stride
	) : cur_(start - stride), end_(end), stride_(stride) {}

	using value_type = std::weak_ptr<int>;
	rusty::Option<value_type> next(
		rusty::type_tag_t<rusty::Iterator<value_type>>
	) {
		if (cur_ >= end_) {
			return rusty::None;
		}
		cur_ += stride_;
		if (cur_ >= end_) {
			return rusty::None;
		}
		returned_ = std::make_shared<int>(cur_);
		return returned_;
	}

private:
	int cur_;
	int end_;
	int stride_;
	std::shared_ptr<int> returned_;
};

class Compare {
public:
	bool operator()(
		const std::weak_ptr<int> &a, const std::weak_ptr<int> &b
	) {
		return *a.lock() < *b.lock();
	}
};

void check(
	std::unique_ptr<rusty::Iterator<std::weak_ptr<int>>> iter,
	int start, int end, int stride
) {
	for (; start < end; start += stride) {
		auto ret = iter->next();
		ASSERT_TRUE(ret.is_some());
		auto x = std::move(ret).unwrap_unchecked().lock();
		ASSERT_TRUE(x != nullptr);
		ASSERT_EQ(*x, start);
	}
	ASSERT_TRUE(iter->next().is_none());
}

}

TEST_F(Test, MergingIteratorSimple) {
	std::vector<int> a{0, 2, 4, 6, 8};
	std::vector<int> b{1, 3, 5, 7, 9};
	std::vector<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	{
		std::vector<
			std::unique_ptr<rusty::Peek<rusty::Ref<const int>>>
		> iters;
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_TRUE(v.empty());
	}
	{
		std::vector<
			std::unique_ptr<rusty::Peek<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(a.data(), a.data()))
		));
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_TRUE(v.empty());
	}
	{
		std::vector<
			std::unique_ptr<rusty::Peek<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(a))
		));
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_NO_FATAL_FAILURE(check_equal(v, a));
	}
	{
		std::vector<
			std::unique_ptr<rusty::Peek<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(a.data(), a.data()))
		));
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(b.data(), b.data()))
		));
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_TRUE(v.empty());
	}
	{
		std::vector<
			std::unique_ptr<rusty::Peek<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(a)))
		);
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(b.data(), b.data()))
		));
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_NO_FATAL_FAILURE(check_equal(v, a));
	}
	{
		std::vector<
			std::unique_ptr<rusty::Peek<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(a.data(), a.data()))
		));
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(b))
		));
		std::vector<rusty::Ref<const int>> v;
		rusty::collect_into(rusty::NewMergingIterator(std::move(iters)), v);
		ASSERT_NO_FATAL_FAILURE(check_equal(v, b));
	}
	{
		std::vector<
			std::unique_ptr<rusty::Peek<rusty::Ref<const int>>>
		> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(a))
		));
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(rusty::slice::MakeIter(b))
		));
		std::vector<rusty::Ref<const int>> v;
		auto iter = rusty::NewMergingIterator(std::move(iters));
		rusty::collect_into(std::move(iter), v);
		ASSERT_NO_FATAL_FAILURE(check_equal(v, c));
	}

	// Test whether MergingIterator supports iterators that return temporary
	// references
	{
		std::vector<std::unique_ptr<rusty::Peek<std::weak_ptr<int>>>> iters;
		auto iter = rusty::NewMergingIterator(std::move(iters), Compare());
		ASSERT_NO_FATAL_FAILURE(check(std::move(iter), 0, 0, 1));
	}
	{
		std::vector<std::unique_ptr<rusty::Peek<std::weak_ptr<int>>>> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(Iterator(0, 0, 1))
		));
		auto iter = rusty::NewMergingIterator(std::move(iters), Compare());
		ASSERT_NO_FATAL_FAILURE(check(std::move(iter), 0, 0, 1));
	}
	{
		std::vector<std::unique_ptr<rusty::Peek<std::weak_ptr<int>>>> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(Iterator(0, 10, 2))
		));
		auto iter = rusty::NewMergingIterator(std::move(iters), Compare());
		ASSERT_NO_FATAL_FAILURE(check(std::move(iter), 0, 10, 2));
	}
	{
		std::vector<std::unique_ptr<rusty::Peek<std::weak_ptr<int>>>> iters;
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(Iterator(0, 10, 2))
		));
		iters.push_back(rusty::NewPeek(
			rusty::MakePeekable(Iterator(1, 10, 2))
		));
		auto iter = rusty::NewMergingIterator(std::move(iters), Compare());
		ASSERT_NO_FATAL_FAILURE(check(std::move(iter), 0, 10, 1));
	}
}
