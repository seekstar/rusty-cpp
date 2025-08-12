#include "rusty/iter/iterator.h"
#include "test.h"

#include <gtest/gtest.h>

namespace {
void check(const std::vector<int> &a, const std::vector<rusty::Ref<const int>> &b) {
	ASSERT_EQ(a.size(), b.size());
	for (size_t i = 0; i < a.size(); ++i) {
		ASSERT_EQ(a[i], b[i].deref());
	}
}
}

TEST_F(Test, IteratorSimple) {
	std::vector<int> a{1, 3, 8, 2};
	ASSERT_TRUE(rusty::MakeIter(a.data(), a.data()).next().is_none());

	std::vector<rusty::Ref<const int>> b;
	rusty::collect_into(rusty::MakeIter(a.data(), a.data() + a.size()), b);
	ASSERT_NO_FATAL_FAILURE(check(a, b));

	std::unique_ptr<rusty::Iterator<rusty::Ref<const int>>> iter =
		rusty::MakeTraitObject(rusty::MakeIter(a.data(), a.data()));
	ASSERT_TRUE(iter->next().is_none());

	iter = rusty::MakeTraitObject(rusty::MakeIter(a.data(), a.data() + a.size()));
	b.clear();
	rusty::collect_into(std::move(iter), b);
	ASSERT_NO_FATAL_FAILURE(check(a, b));
}
